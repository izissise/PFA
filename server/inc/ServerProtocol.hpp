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

#define LOGFILE "../log/ClientInfo.txt"

using namespace google::protobuf;

class ServerProtocol
{
public:
  ServerProtocol(World &world);
  ~ServerProtocol();

  void  parseCmd(const void *data, int size,
		 Client *client,
		 const std::vector<Client *> &clients);

private:
  typedef void (ServerProtocol::*parseFunc)(ClientMessage &message,
					    Client *client,
					    const std::vector<Client *> &clients);

  void  handleConnection(ClientMessage &message,
			 Client *client,
			 const std::vector<Client *> &clients);
  void	handleActions(ClientMessage &message,
		      Client *client,
		      const std::vector<Client *> &clients);
  void	queryChunks(ClientMessage &message,
		    Client *client,
		    const std::vector<Client *> &clients);

  void	loadClientProfile(const std::vector<Client *> &clients,
			  Client *client, const std::string &userId);
  void	spawnClient(const std::vector<Client *> &clients,
		    Client *Client);
  int	isChunkSpawnable(Chunk *chunk,
			 const std::vector<Client *> &clients);
  bool	placePlayerOnSurface(Chunk *chunk,
			     unsigned int chunkPart,
			     Vector2u &chunkPos);
  void	sendClientProfile(Client *client, const std::string &newId);
  void	generateNewId(std::string &guid);

  std::map<ClientMessage::PacketContent, parseFunc> _func;
  World		&_world;
};

#endif
