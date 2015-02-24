#include "ServerProtocol.hpp"
#include "StringUtils.hpp"
#include "GuidGenerator.hpp"
#include "Spawner.hpp"

ServerProtocol::ServerProtocol(World &world, ThreadPool &threadPool) :
  _world(world), _threadPool(threadPool)
{
  _func[ClientMessage::ACTION] = &ServerProtocol::handleActions;
  _func[ClientMessage::QUERYCHUNK] = &ServerProtocol::queryChunks;
  _func[ClientMessage::GETPLAYER] = &ServerProtocol::getPlayer;
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
      {
	std::cout << (int)act << std::endl;
	if (act == ClientMessage::QUERYCHUNK)
	  _threadPool.addTask([this, &clients, packet, client]()
			      {
				queryChunks(packet, client, clients);
			      });
	else
	  (this->*(it->second))(packet, client, clients);
      }
  }
  else
    std::cerr << "Cannot DeSerialize Data" << std::endl;
}

void	ServerProtocol::handleActions(const ClientMessage &message,
				      Client *client,
				     UNUSED const std::vector<Client *> &clients)
{
  const ClientActions	&clientActions = message.actions();
  unsigned int		nbActions = clientActions.actions_size();

  for (unsigned int i = 0; i < nbActions; ++i)
    {
      const ClientAction	&clAction = clientActions.actions(i);

      client->setAction(clAction.name(), clAction.state());
    }
}

void	ServerProtocol::queryChunks(const ClientMessage &message,
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
  for (auto &chunkId : newChunks)
    client->sendPacket(2, _world.serialize(chunkId)); // send on 2 because it's a huge transfer
}

void    ServerProtocol::getPlayer(const ClientMessage &message,
                                  Client *client,
                                  const std::vector<Client *> &clients)
{
    ServerResponse response;
    std::string str;


    response.set_content(ServerResponse::PLAYER);
    response.set_player(clients.size() - 1);
    response.SerializeToString(&str);

    std::cout << "Nb Player = " << response.player() << std::endl;
    for (unsigned int i = 0; i < str.length();++i)
    {
        std::cout << (int)str.data()[i] << " ";
    }
    std::cout << std::endl;
    client->sendPacket(0, str);
}
