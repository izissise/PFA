#ifndef _CLIENTPROTOCOL_HPP_
# define _CLIENTPROTOCOL_HPP_

# include <iostream>
# include "World.hpp"
# include "Settings.hpp"
# include "ProtocolMessage.pb.h"

class ClientProtocol
{
public:
  ClientProtocol();
  ~ClientProtocol();
  
  void  parseCmd(const void *data, int size);
  void  setSetting(Settings *set);
  void  setWorld(const std::shared_ptr<World> &world);

  
private:
  void  handleSetting(ProtocolMessage packet);
  
  std::map<ProtocolMessage::Action, void (ClientProtocol::*)(ProtocolMessage packet)> _func;
  std::shared_ptr<World>     _world;
  Settings  *_set;
};

#endif