#include <fstream>
#include "ClientProtocol.hpp"

ClientProtocol::ClientProtocol(Network &net, ThreadPool &tPool, Chat &chat) :
  _socket(net), _threadPool(tPool), _chat(chat)
{
  _func[ProtocolMessage::SETTING] = &ClientProtocol::handleSetting;
  _func[ProtocolMessage::CLINIT] = &ClientProtocol::initClient;
  _func[ProtocolMessage::CLSPAWN] = &ClientProtocol::spawnClient;
  _func[ProtocolMessage::CHUNK] = &ClientProtocol::fillChunk;
  _func[ProtocolMessage::DISPLACEMENT] = &ClientProtocol::handleDisplacements;
  _func[ProtocolMessage::CHAT] = &ClientProtocol::handleChat;
}

ClientProtocol::~ClientProtocol()
{
}

void ClientProtocol::setSetting(Settings *set)
{
  _set = set;
}

void ClientProtocol::setWorld(const std::shared_ptr<World> &world)
{
  _world = world;
}

void  ClientProtocol::parseCmd(const void *data, int size)
{
  ProtocolMessage          packet;

  // std::cout << "size: " << size << std::endl;
  if (packet.ParseFromString(std::string((char *)data, size)))
    {
      ProtocolMessage::PacketContent  act = packet.content();
      auto it = _func.find(act);

      if (it != _func.end())
	{
	  if (act == ProtocolMessage::CHUNK)
	    _threadPool.addTask([&, packet, it]()
				{ (this->*(it->second))(packet); });
	  else
	    (this->*(it->second))(packet);
	}
    }
  else
    std::cerr << "Cannot DeSerialize Data" << std::endl;
}

void  ClientProtocol::handleSetting(const ProtocolMessage &packet)
{
  if (!packet.has_settings())
    return ;
  const SettingMessage &set = packet.settings();

  for (int i = 0; i < set.cvars_size(); ++i)
    {
      const cvarData	&cvar = set.cvars(i);
      std::vector<std::string>	restrict(cvar.restrictedvalues().begin(),
					 cvar.restrictedvalues().end());

      t_cvar *entry = new t_cvar(restrict,
				 std::string(cvar.default_()),
				 cvar.value(),
				 static_cast<cvarType>(cvar.type()),
				 static_cast<Restriction>(cvar.restrictiontype()));

      if (!_set->getCvarList().addCvar(cvar.key(), entry))
	{
	  _set->getCvarList().setCvar(cvar.key(), cvar.value());
	  delete entry;
	}
    }
}

void	ClientProtocol::initClient(const ProtocolMessage &packet)
{
  if (!packet.has_clinit())
    return ;
  std::cout << "Client Initialization" << std::endl;

  const InitClient	&initData = packet.clinit();
  const	Position	&pos = initData.pos();
  const VectorInt	&chunkId = pos.chunkid();
  const VectorFloat	&playerPos = pos.pos();
  const std::string	&guid = initData.guid();

  _world->setPlayerPosition(Vector2i(chunkId.x(), chunkId.y()),
			    Vector2f(playerPos.x(), playerPos.y()));
  if (!guid.empty())
    writeNewGuid(guid);
  queryInitialChunks();
}

void	ClientProtocol::spawnClient(const ProtocolMessage &packet)
{
  if (!packet.has_clinfo())
    return ;
  const PBClientInfo	&clInfo = packet.clinfo();
  const Position	&position = clInfo.posinfo();
  const VectorFloat	&chunkPos = position.pos();
  const VectorInt	&chunkId = position.chunkid();

  std::cout << "spawn client at pos: " << chunkId.x() << " " << chunkId.y() << " in "
	    << chunkPos.x() << " " << chunkPos.y() << std::endl;
}

void		ClientProtocol::writeNewGuid(const std::string &guid) const
{
  std::ofstream	file;

  file.open(GUIDFILE, std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);
  if (file)
    {
      file << guid << std::endl;
      file.close();
    }
}

void			ClientProtocol::queryInitialChunks()
{
  Vector2i		chunkPos;
  Vector2u		sideSize;
  std::vector<Vector2i>	chunks;
  const Player		&player = _world->getPlayer();
  Vector2i		res(std::stoi(_set->getCvarList().getCvar("r_width")),
			    std::stoi(_set->getCvarList().getCvar("r_height")));
  std::function<int (int num, int factor)> roundFunc = [](int num, int factor)
    -> int
    {
      return (num == 0 ? 0 : num + factor - 1 - (num - 1) % factor);
    };

  chunkPos = player.getChunkId();
  sideSize.x = 2 + roundFunc(res.x / Chunk::pWidth, 2) + 1;
  sideSize.y = 2 + roundFunc(res.y / Chunk::pHeight, 2) + 1;

  std::cout << "Plpos: " << chunkPos.x << " " << chunkPos.y
	    << " | " << player.getPosition().x << " " << player.getPosition().y << std::endl;
  std::cout << "W/H: " << sideSize.x  << " " << sideSize.y << std::endl;
  for (int y = chunkPos.y - (sideSize.y - 1) / 2;
       y <= chunkPos.y + (static_cast<int>(sideSize.y) - 1) / 2; ++y)
    {
      for (int x = chunkPos.x - (sideSize.x - 1) / 2;
  	   x <= chunkPos.x + (static_cast<int>(sideSize.x) - 1) / 2; ++x)
	chunks.push_back({x, y});
    }
  queryChunks(chunks);
}

void			ClientProtocol::queryChunks(const std::vector<Vector2i> &chunkIds) const
{
  ClientMessage		msg;
  QueryChunk		*qChunk = new QueryChunk;
  VectorInt		*id;
  std::string		serialized;

  for (auto &chunkId : chunkIds)
    {
      id = qChunk->add_id();
      id->set_x(chunkId.x);
      id->set_y(chunkId.y);
      std::cout << "Query chunk -> " << chunkId.x << " " << chunkId.y << std::endl;
    }
  msg.set_content(ClientMessage::QUERYCHUNK);
  msg.set_allocated_querychunk(qChunk);
  msg.SerializeToString(&serialized);
  _socket.sendPacket(1, serialized);
}

void	ClientProtocol::fillChunk(const ProtocolMessage &packet)
{
  if (!packet.has_chunkdata())
    return ;

  const ChunkData	&chunk = packet.chunkdata();
  const VectorInt	&chunkId = chunk.id();

  _world->fillChunkData(chunkId, chunk);
}

void			ClientProtocol::getNewChunks()
{
  std::vector<Vector2i>	chunks;

  if (_world->getNewChunks(chunks))
    queryChunks(chunks);
}

void	ClientProtocol::handleDisplacements(const ProtocolMessage &packet)
{
  if (!packet.has_displacement())
    return ;

  const Displacement	&displacement = packet.displacement();
  const	Position	&position = displacement.position();
  const	VectorInt	&chunkId = position.chunkid();
  const VectorFloat	&pos = position.pos();

  if (_world->movePlayer(chunkId, pos))
    getNewChunks();
}

void	ClientProtocol::handleChat(const ProtocolMessage &packet)
{
  _chat.addMessages(packet.chat());
}
