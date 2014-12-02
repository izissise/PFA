#ifndef _SERVERPROTOCOL_HPP_
# define _SERVERPROTOCOL_HPP_

#include <iostream>
#include "ProtocolMessage.pb.h"

class ServerProtocol
{
public:
  ServerProtocol();
  ~ServerProtocol();
  
	void  parseCmd(const void *data, int size);

private:
  void  handleConnection();
  void  handleLogin();
  
  std::map<ProtocolMessage::Action, void (ServerProtocol::*)()> _func;
};

#endif