#include "ServerProtocol.hpp"

ServerProtocol::ServerProtocol()
{
  _func[ProtocolMessage::CONNECTION] = &ServerProtocol::handleConnection;
  _func[ProtocolMessage::LOGIN] = &ServerProtocol::handleLogin;
}

ServerProtocol::~ServerProtocol()
{
  
}

void  ServerProtocol::parseCmd(const void *data, int size)
{
	ProtocolMessage          tmp;

  if (tmp.ParseFromString(std::string((char *)data, size)))
  {
    ProtocolMessage::Action  act = tmp.action();

    if (_func.find(act) != _func.end())
      (this->*_func[act])();
  }
	else
		std::cerr << "Cannot DeSerialize Data" << std::endl;
}

void  ServerProtocol::handleConnection()
{
  std::cout << "CONNECTION" << std::endl;
  
}

void  ServerProtocol::handleLogin()
{
  std::cout << "LOGIN" << std::endl;
}

