#include <fstream>
#include "ServerProtocol.hpp"
#include "StringUtils.hpp"

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
  loadClientProfile(client, userId);
  sendClientProfile(client);
}

void	ServerProtocol::handleActions(ClientMessage &message,
				      Client *client,
				      const std::vector<Client *> &clients)
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

void	ServerProtocol::spawnClient(Client *client)
{
  ClientEntity	&clEnt = client->getEntity();

  std::cout << "New client -> Spawn init" << std::endl;
  //easy placement
  clEnt.setChunkId({0,0});
  clEnt.setPosition({0,0});
}

void	ServerProtocol::loadClientProfile(Client *client, const std::string &userId)
{
  std::fstream			file;
  StringUtils			utils;
  std::vector<std::string>	clientInfo;
  std::vector<std::string>	vec;
  ClientEntity			&clEnt = client->getEntity();

  file.open(LOGFILE, std::ios::binary | std::ios::in);
  if (!file) // Can be the first client joining
    {
      std::cout << "File doesnt exist yet" << std::endl;
      spawnClient(client);
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
  spawnClient(client);
}

void	ServerProtocol::sendClientProfile(Client *client)
{
  ProtocolMessage	msg;
  InitClient		*initInfo = new InitClient;
  Position		*pos = new Position;
  VectorInt		*chunkId = new VectorInt;
  VectorFloat		*clPos = new VectorFloat;
  ClientEntity		&clEnt = client->getEntity();
  std::string		serialized;

  chunkId->set_x(clEnt.getChunkId().x);
  chunkId->set_y(clEnt.getChunkId().y);
  pos->set_allocated_chunkid(chunkId);

  clPos->set_x(clEnt.getPosition().x);
  clPos->set_y(clEnt.getPosition().y);
  pos->set_allocated_pos(clPos);

  initInfo->set_allocated_pos(pos);
  msg.set_content(ProtocolMessage::CLINIT);
  msg.set_allocated_clinit(initInfo);
  msg.SerializeToString(&serialized);
  client->sendPacket(0, serialized);
}
