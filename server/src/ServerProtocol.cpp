#include "ServerProtocol.hpp"

ServerProtocol::ServerProtocol(World &world) :
  _world(world)
{
  _func[ProtocolMessage::CONNECTION] = &ServerProtocol::handleConnection;
  _func[ProtocolMessage::LOGIN] = &ServerProtocol::handleLogin;
  _func[ProtocolMessage::ACTION] = &ServerProtocol::handleActions;
}

ServerProtocol::~ServerProtocol()
{
}

void	ServerProtocol::parseCmd(const void *data, int size,
				 Client *client,
				 const std::vector<Client *> &clients)
{
  ProtocolMessage          packet;

  if (packet.ParseFromString(std::string((char *)data, size)))
  {
    ProtocolMessage::PacketContent  act = packet.content();
    auto it = _func.find(act);

    if (it != _func.end())
      (this->*(it->second))(packet, client, clients);
  }
  else
    std::cerr << "Cannot DeSerialize Data" << std::endl;
}

void  ServerProtocol::handleConnection(ProtocolMessage &message,
				       Client *client,
				       const std::vector<Client *> &clients)
{
  ClientInfo			&clInfo = client->getInfo();
  ClientEntity			&clEnt = client->getEntity();
  const ConnectionMessage	&coInfo = message.co();
  const VectorUint		&screenRes = coInfo.screenres();

  Vector2i			chunkPos;
  std::vector<Vector2i>		chunks;
  Vector2i			sideSize;

  std::cout << "CONNECTION" << std::endl;
  clInfo.setResolution(Vector2u(screenRes.x(), screenRes.y()));

  chunkPos = clEnt.getChunkId();
  // +1 is the Center, X * 2 for what is bordering it, + 2 for the sides
  sideSize.x = (screenRes.x() / (Chunk::width * TileCodex::tileSize) * 2) + 1 + 2;
  sideSize.y = (screenRes.y() / (Chunk::height * TileCodex::tileSize) * 2) + 1 + 2;
  for (int y = chunkPos.y - (sideSize.y - 1) / 2;
       y <= chunkPos.y + (sideSize.y - 1) / 2; ++y)
    {
      for (int x = chunkPos.x - (sideSize.x - 1) / 2;
	   x <= chunkPos.x + (sideSize.x - 1) / 2; ++x)
	{
	  _world.loadChunk(clients, x, y);
	  chunks.push_back({x, y});
	}
    }
  client->sendPacket(0, _world.serialize(chunks));
}

void  ServerProtocol::handleLogin(ProtocolMessage &message,
				  Client *client,
				  const std::vector<Client *> &clients)

{
  std::cout << "LOGIN" << std::endl;
}

void	ServerProtocol::handleActions(ProtocolMessage &message,
				      Client *client,
				      const std::vector<Client *> &clients)
{
  const ClientActions	&clientActions = message.actions();
  unsigned int		nbActions = clientActions.actions_size();
  unsigned int		actionId;

  for (unsigned int i = 0; i < nbActions; ++i)
    {
      const ClientAction	&clAction = clientActions.actions(i);

      std::cout << clAction.name() << " " << clAction.state() << std::endl;
    }
}
