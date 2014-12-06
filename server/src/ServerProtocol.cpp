#include "ServerProtocol.hpp"

ServerProtocol::ServerProtocol()
{
  _func[ProtocolMessage::CONNECTION] = &ServerProtocol::handleConnection;
  _func[ProtocolMessage::LOGIN] = &ServerProtocol::handleLogin;
  _func[ProtocolMessage::ACTION] = &ServerProtocol::handleActions;
}

ServerProtocol::~ServerProtocol()
{
}

void	ServerProtocol::parseCmd(const void *data, int size)
{
  ProtocolMessage          tmp;

  if (tmp.ParseFromString(std::string((char *)data, size)))
  {
    ProtocolMessage::PacketContent  act = tmp.content();

    if (_func.find(act) != _func.end())
      (this->*_func[act])(tmp);
  }
  else
    std::cerr << "Cannot DeSerialize Data" << std::endl;
}

void  ServerProtocol::handleConnection(ProtocolMessage &message)
{
  std::cout << "CONNECTION" << std::endl;
}

void  ServerProtocol::handleLogin(ProtocolMessage &message)
{
  std::cout << "LOGIN" << std::endl;
}

void	ServerProtocol::handleActions(ProtocolMessage &message)
{
  const ClientActions	&actions = message.actions();


}
