#include <fstream>
#include "ServerProtocol.hpp"
#include "StringUtils.hpp"
#include "GuidGenerator.hpp"

ServerProtocol::ServerProtocol(World &world) :
  _world(world)
{
  _func[ClientMessage::CONNECTION] = &ServerProtocol::handleConnection;
  _func[ClientMessage::ACTION] = &ServerProtocol::handleActions;
  _func[ClientMessage::QUERYCHUNK] = &ServerProtocol::queryChunks;
}

ServerProtocol::~ServerProtocol()
{
}

void	ServerProtocol::parseCmd(const void *data, int size,
				 Client *client,
				 const std::vector<Client *> &clients)
{
  ClientMessage          packet;

  if (packet.ParseFromString(std::string((char *)data, size)))
  {
    ClientMessage::PacketContent  act = packet.content();
    auto it = _func.find(act);

    if (it != _func.end())
      (this->*(it->second))(packet, client, clients);
  }
  else
    std::cerr << "Cannot DeSerialize Data" << std::endl;
}

void  ServerProtocol::handleConnection(ClientMessage &message,
				       Client *client,
				       const std::vector<Client *> &clients)
{
  ClientInfo			&clInfo = client->getInfo();
  ClientEntity			&clEnt = client->getEntity();
  const ConnectionMessage	&coInfo = message.co();
  const std::string		&userId = coInfo.userid();
  std::string			newId;

  std::cout << "USERID " << userId << std::endl;
  if (userId.empty())
    {
      generateNewId(newId);
      client->getInfo().setId(newId);
    }
  else
    client->getInfo().setId(userId);
  auto it = std::find_if(clients.begin(), clients.end(), [client, &userId](Client *cl)
			 {
			   return (client != cl && cl->getInfo().getId() == userId);
			 });
  if (it != clients.end())
    {
      enet_peer_disconnect(client->getPeer(), 0);
      return ;
    }
  loadClientProfile(clients, client, userId);
  client->initialize();
  sendClientProfile(client, newId);
}

void	ServerProtocol::handleActions(ClientMessage &message,
				      Client *client,
				      const std::vector<Client *> &clients UNUSED)
{
  const ClientActions	&clientActions = message.actions();
  unsigned int		nbActions = clientActions.actions_size();

  for (unsigned int i = 0; i < nbActions; ++i)
    {
      const ClientAction	&clAction = clientActions.actions(i);

      client->setAction(clAction.name(), clAction.state());
    }
}

void	ServerProtocol::queryChunks(ClientMessage &message,
				    Client *client,
				    const std::vector<Client *> &clients)
{
  const QueryChunk	&qChunks = message.querychunk();
  const RepeatedPtrField<VectorInt>	&chunkIds = qChunks.id();
  std::vector<Vector2i>	newChunks;
  unsigned int		idx = 0;
  Vector2i		pos;

  newChunks.reserve(chunkIds.size());
  std::cout << "Query Chunk" << std::endl;
  for (auto &id : chunkIds)
    {
      pos = {id.x(), id.y()};
      if (_world.isChunkLoaded(pos) == false)
	_world.loadChunk(clients, pos);
      newChunks.emplace(newChunks.begin() + idx, pos);
      ++idx;
    }
  client->sendPacket(0, _world.serialize(newChunks));
}

void		ServerProtocol::generateNewId(std::string &guid)
{
  GuidGenerator	gen;

  guid = gen.generate();
}

int			ServerProtocol::isChunkSpawnable(Chunk *chunk,
							 const std::vector<Client *> &clients)
{
  for (unsigned int idx = 0; idx < Chunk::lod; ++idx)
    {
      const t_ChunkInfo	&cInfo = chunk->getChunkInfo(idx);

      if (cInfo.avHeight > 0 && cInfo.avHeight < (3 * Chunk::height) / 4)
	return idx;
    }
  return -1;
}

bool		ServerProtocol::placePlayerOnSurface(Chunk *chunk,
						     unsigned int chunkPart,
						     Vector2u &chunkPos)
{
  const std::vector<TileType>	&tiles = chunk->getTiles();
  unsigned int	xPad;
  unsigned int	lodSize;

  lodSize = (Chunk::width * Chunk::lineLenght) / Chunk::lod;
  xPad = chunkPart * lodSize;
  std::cout << "XPAD: " << xPad << std::endl;
  for (unsigned int pos = xPad; pos < xPad + lodSize; ++pos)
    {
      if (tiles[pos] != TileType::Empty)
	continue ;
      for (unsigned int y = Chunk::height - 1; y > 0; --y)
	{
	  std::cout << "Chunk Tiles pos " << pos << " " << y << " -> "
		    << (int)tiles[y * Chunk::width + pos] << " "
		    << (int)tiles[(y - 1) * Chunk::width + pos] << std::endl;
	  if (tiles[y * Chunk::width + pos] == TileType::Empty &&
	      tiles[(y - 1) * Chunk::width + pos] != TileType::Empty)
	    {
	      chunkPos = {pos, y};
	      return true;
	    }
	}
    }
  return false;
}

void		ServerProtocol::spawnClient(const std::vector<Client *> &clients,
					    Client *client)
{
  ClientEntity	&clEnt = client->getEntity();
  Vector2i	chunkId(0,1);
  Vector2u	plPos;
  Chunk		*chunk;
  int		chunkPart;

  std::cout << "New client -> Spawn init" << std::endl;
  for (int dist = 0; true; dist++)
    {
      for (int side = -dist; side <= dist; side += (dist * 2))
	{
	  chunkId.x = side;
	  if (!_world.isChunkLoaded(chunkId))
	    chunk = _world.loadChunk(chunkId, clients, chunkId);
	  else
	    chunk = _world.getChunk(chunkId);
	  if ((chunkPart = isChunkSpawnable(chunk, clients)) != -1)
	    if (placePlayerOnSurface(chunk, chunkPart, plPos) == true)
	      {
		clEnt.setChunkId(chunk->getPosition());
		clEnt.setPosition(Vector2f(static_cast<float>(plPos.x) / Chunk::width,
					   static_cast<float>(plPos.y) / Chunk::height));
		return ;
	      }
	  if (dist == 0)
	    break ;
	}
    }
}

void	ServerProtocol::loadClientProfile(const std::vector<Client *> &clients,
					  Client *client, const std::string &userId)
{
  std::fstream			file;
  StringUtils			utils;
  std::vector<std::string>	clientInfo;
  std::vector<std::string>	vec;
  ClientEntity			&clEnt = client->getEntity();

  if (userId.empty())
    {
      spawnClient(clients, client);
      return ;
    }
  file.open(LOGFILE, std::ios::binary | std::ios::in);
  if (!file) // Can be the first client joining
    {
      std::cout << "File doesnt exist yet" << std::endl;
      spawnClient(clients, client);
      return ;
    }
  for (std::string line; getline(file, line);)
    {
      if (line.find(userId) != std::string::npos)
	{
	  std::cout << "Found in database -> load client's data" << std::endl;
	  utils.split(line, ';', clientInfo);
	  utils.split(clientInfo.at(1), ' ', vec);
	  clEnt.setChunkId({std::stoi(vec.at(0)), std::stoi(vec.at(1))});
	  vec.clear();
	  utils.split(clientInfo.at(2), ' ', vec);
	  clEnt.setPosition({std::stof(vec.at(0)), std::stof(vec.at(1))});
	  file.close();
	  return ;
	}
    }
  file.close();
  spawnClient(clients, client);
}

void	ServerProtocol::sendClientProfile(Client *client,
					  const std::string &newId)
{
  ProtocolMessage	msg;
  InitClient		*initInfo = new InitClient;
  Position		*pos = new Position;
  VectorInt		*chunkId = new VectorInt;
  VectorFloat		*clPos = new VectorFloat;
  std::string		*guid = new std::string(newId);
  ClientEntity		&clEnt = client->getEntity();
  std::string		serialized;

  chunkId->set_x(clEnt.getChunkId().x);
  chunkId->set_y(clEnt.getChunkId().y);
  pos->set_allocated_chunkid(chunkId);

  clPos->set_x(clEnt.getPosition().x);
  clPos->set_y(clEnt.getPosition().y);
  pos->set_allocated_pos(clPos);

  initInfo->set_allocated_pos(pos);
  initInfo->set_allocated_guid(guid);
  msg.set_content(ProtocolMessage::CLINIT);
  msg.set_allocated_clinit(initInfo);
  msg.SerializeToString(&serialized);
  client->sendPacket(0, serialized);
}
