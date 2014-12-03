#ifndef _CLIENTPROTOCOL_HPP_
# define _CLIENTPROTOCOL_HPP_

# include <iostream>
# include "ProtocolMessage.pb.h"

class ClientProtocol
{
public:
  ClientProtocol();
  ~ClientProtocol();
  
  void  parseCmd(const void *data, int size);
  
private:
  void  handleSetting(ProtocolMessage packet);
  
  std::map<ProtocolMessage::Action, void (ClientProtocol::*)(ProtocolMessage packet)> _func;
};

#endif