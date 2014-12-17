#ifndef Client_H_
#define Client_H_

#include <iostream>
#include <enet/enet.h>
#include <vector>

#include "NetworkException.hpp"
#include "ClientEntity.hpp"
#include "ClientInfo.hpp"
#include "ActionEnum.hpp"

class Client
{
public:
  Client(ENetPeer *peer);
  ~Client();

  void sendPacket(enet_uint8 chan, const std::string &packet) const;

  ENetPeer		*getPeer();
  ClientEntity		&getEntity();
  ClientInfo		&getInfo();
  bool			isInitialized() const;

  void			setAction(unsigned int actionIdx, bool state);
  const std::vector<bool> &getActions() const;
  void			initialize();

private:
  ENetPeer		*_peer;
  ClientEntity		_entity;
  ClientInfo		_info;
  std::vector<bool>	_actions;
  bool			_initialized;
};

#endif
