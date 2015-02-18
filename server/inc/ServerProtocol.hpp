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

#define LOGFILE "../log/ClientInfo.txt"

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

  void  handleConnection(const ClientMessage &message,
			 Client *client,
			 const std::vector<Client *> &clients);
  void	handleActions(const ClientMessage &message,
		      Client *client,
		      const std::vector<Client *> &clients);
  void	queryChunks(const ClientMessage &message,
		    Client *client,
		    const std::vector<Client *> &clients);
  void	ping(const ClientMessage &message,
	     Client *client,
	     const std::vector<Client *> &clients);
  void	spawnClient(const std::vector<Client *> &clients,
		    Client *client);
  void	loadClientProfile(const std::vector<Client *> &clients,
			  Client *client, const std::string &userId);
  void	sendClientProfile(Client *client, const std::string &newId);
  void	generateNewId(std::string &guid);

  std::map<ClientMessage::PacketContent, parseFunc> _func;
  World		&_world;
  ThreadPool	&_threadPool;
};

#endif
