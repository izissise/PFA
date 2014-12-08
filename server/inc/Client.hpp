#ifndef Client_H_
#define Client_H_

#include <iostream>
#include <enet/enet.h>
#include "NetworkException.hpp"
#include "ClientEntity.hpp"
#include "ClientInfo.hpp"

class Client
{
public:
  Client(ENetPeer *peer);
  ~Client();

  void sendPacket(enet_uint8 chan, const std::string &packet) const;

  const ENetPeer	*getPeer() const;
  ClientEntity		&getEntity();
  ClientInfo		&getInfo();

private:
  ENetPeer	*_peer;
  ClientEntity	_entity;
  ClientInfo	_info;
};

#endif
