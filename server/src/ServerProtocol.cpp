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
				 const ENetPeer *peer,
				 const std::vector<Client *> &clients)
{
  ProtocolMessage          packet;

  if (packet.ParseFromString(std::string((char *)data, size)))
  {
    ProtocolMessage::PacketContent  act = packet.content();
    auto it = _func.find(act);

    if (it != _func.end())
      (this->*(it->second))(packet, peer, clients);
  }
  else
    std::cerr << "Cannot DeSerialize Data" << std::endl;
}

void  ServerProtocol::handleConnection(ProtocolMessage &message,
				       const ENetPeer *peer,
				       const std::vector<Client *> &clients)
{
  std::cout << "CONNECTION" << std::endl;
}

void  ServerProtocol::handleLogin(ProtocolMessage &message,
				  const ENetPeer *peer,
				  const std::vector<Client *> &clients)

{
  std::cout << "LOGIN" << std::endl;
}

void	ServerProtocol::handleActions(ProtocolMessage &message,
				      const ENetPeer *peer,
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
