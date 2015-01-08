#include <fstream>
#include "ClientProtocol.hpp"

ClientProtocol::ClientProtocol(Network &net, ThreadPool &tPool) :
  _socket(net), _threadPool(tPool)
{
  _func[ProtocolMessage::SETTING] = &ClientProtocol::handleSetting;
  _func[ProtocolMessage::CLINIT] = &ClientProtocol::initClient;
  _func[ProtocolMessage::CHUNK] = &ClientProtocol::fillChunk;
  _func[ProtocolMessage::DISPLACEMENT] = &ClientProtocol::handleDisplacements;
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

  if (packet.ParseFromString(std::string((char *)data, size)))
    {
      ProtocolMessage::PacketContent  act = packet.content();
      auto it = _func.find(act);

      if (it != _func.end())
	{
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
  std::cout << "Handle Setting" << std::endl;

  SettingMessage set = packet.settings();
  for (int i = 0; i < set.settingentry_size(); ++i)
    {
      SettingMessage::SettingEntry::Cvar cvar = set.settingentry(i).cvar();
      t_cvar *entry = new t_cvar({{std::string(cvar.default_()),
	      std::string(cvar.min()),
	      std::string(cvar.max())}},
	cvar.value(), static_cast<cvarType>(cvar.type()));

      if (!_set->getCvarList().addCvar(set.settingentry(i).key(), entry))
	{
	  _set->getCvarList().setCvar(set.settingentry(i).key(), cvar.value());
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
  float			rWidth;
  float			rHeight;

  rWidth = std::stof(_set->getCvarList().getCvar("r_width"));
  rHeight = std::stof(_set->getCvarList().getCvar("r_height"));
  // +1 is the Center, X * 2 for what is bordering it, + 2 for the sides
  chunkPos = player.getChunkId();
  sideSize.x = 2 + std::ceil(rWidth / Chunk::pWidth);
  sideSize.y = 2 + std::ceil(rHeight / Chunk::pHeight);

  // sideSize.x =  1 + (std::stoi(_set->getCvarList().getCvar("r_width"))
  // 		     / Chunk::pWidth * 2) + 2;
  // sideSize.y = 1 + (std::stoi(_set->getCvarList().getCvar("r_height"))
  // 		    / Chunk::pHeight * 2) + 2;
  std::cout << "Plpos: " << chunkPos.x << " " << chunkPos.y << std::endl;
  std::cout << "W/H: " << sideSize.x  << " " << sideSize.y << std::endl;
  for (int y = chunkPos.y - (sideSize.y - 1) / 2;
       y <= chunkPos.y + (static_cast<int>(sideSize.y) - 1) / 2; ++y)
    {
      for (int x = chunkPos.x - (sideSize.x - 1) / 2;
  	   x <= chunkPos.x + (static_cast<int>(sideSize.x) - 1) / 2; ++x)
	{
	  std::cout << "creating chunks " << x << " " << y << std::endl;
	  chunks.push_back({x, y});
	}
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
  if (!packet.has_fullchunk())
    return ;

  const FullChunk	&fullChunk = packet.fullchunk();
  unsigned int		nbChunk = fullChunk.chunkdata_size();

  std::cout << "FullChunk packet -> " << nbChunk << std::endl;
  //  _world->removeOldChunks();
  for (unsigned int i = 0; i < nbChunk; ++i)
    {
      const ChunkData	&chunk = fullChunk.chunkdata(i);
      const VectorInt	&chunkId = chunk.id();
      const RepeatedField<uint32> &bgTiles = chunk.bgtiles();
      const RepeatedField<uint32> &fgTiles = chunk.fgtiles();

      _world->fillChunkData(chunkId, bgTiles, fgTiles);
    }
  if (!_world->isLoaded())
    _world->load();
  else
    _world->loadRange();
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
