#ifndef _SERVERPROTOCOL_HPP_
# define _SERVERPROTOCOL_HPP_

#include <iostream>
#include <vector>
#include "ProtocolMessage.pb.h"
#include "World.hpp"
#include "Client.hpp"

class ServerProtocol
{
public:
  ServerProtocol(World &world);
  ~ServerProtocol();

  void  parseCmd(const void *data, int size,
		 const ENetPeer *peer,
		 const std::vector<Client *> &clients);

private:
  typedef void (ServerProtocol::*parseFunc)(ProtocolMessage &message,
					    const ENetPeer *peer,
					    const std::vector<Client *> &clients);
  void  handleConnection(ProtocolMessage &message,
			 const ENetPeer *peer,
			 const std::vector<Client *> &clients);
  void  handleLogin(ProtocolMessage &message,
		    const ENetPeer *peer,
		    const std::vector<Client *> &clients);
  void	handleActions(ProtocolMessage &message,
		      const ENetPeer *peer,
		      const std::vector<Client *> &clients);

  std::map<ProtocolMessage::PacketContent, parseFunc> _func;
  World		&_world;
};

#endif
