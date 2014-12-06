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
  typedef void (ServerProtocol::*parseFunc)(ProtocolMessage &message);
  void  handleConnection(ProtocolMessage &message);
  void  handleLogin(ProtocolMessage &message);
  void	handleActions(ProtocolMessage &message);

  std::map<ProtocolMessage::PacketContent, parseFunc> _func;
};

#endif
