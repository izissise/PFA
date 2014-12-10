#include "ServerProtocol.hpp"

ServerProtocol::ServerProtocol(World &world) :
  _world(world)
{
  _func[ClientMessage::CONNECTION] = &ServerProtocol::handleConnection;
  _func[ClientMessage::ACTION] = &ServerProtocol::handleActions;
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
  std::cout << "CONNECTION, UserId: " << userId <<  std::endl;
}

void	ServerProtocol::handleActions(ClientMessage &message,
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
