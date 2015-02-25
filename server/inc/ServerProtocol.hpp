#ifndef _SERVERPROTOCOL_HPP_
# define _SERVERPROTOCOL_HPP_

#include <iostream>
#include <vector>
#include "ProtocolMessage.pb.h"
#include "ClientMessage.pb.h"
#include "World.hpp"
#include "Client.hpp"
#include "Unused.hpp"
#include "ActionEnum.hpp"
#include "ThreadPool.hpp"

using namespace google::protobuf;

class ServerProtocol
{
public:
  ServerProtocol(World &world, ThreadPool &threadPool);
  ~ServerProtocol();

  void  parseCmd(const void *data, int size,
		 Client *client,
		 const std::vector<Client *> &clients);

private:
  typedef void (ServerProtocol::*parseFunc)(const ClientMessage &message,
					    Client *client,
					    const std::vector<Client *> &clients);

  void	handleActions(const ClientMessage &message,
		      Client *client,
		      const std::vector<Client *> &clients);
  void	queryChunks(const ClientMessage &message,
		    Client *client,
		    const std::vector<Client *> &clients);
  void	chat(const ClientMessage &message,
	     Client *client,
	     const std::vector<Client *> &clients);

  std::map<ClientMessage::PacketContent, parseFunc> _func;
  World		&_world;
  ThreadPool	&_threadPool;
};

#endif
