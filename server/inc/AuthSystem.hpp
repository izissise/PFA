#ifndef _AUTHSYSTEM_H_
#define _AUTHSYSTEM_H_

#include <enet/enet.h>
#include <map>
#include <vector>

#include "Client.hpp"
#include "ProtocolMessage.pb.h"
#include "ClientMessage.pb.h"
#include "World.hpp"
#include "Unused.hpp"
#include "Observer.hpp"

#define LOGFILE "../log/ClientInfo.txt"

typedef struct  s_event
{
  Client	*client;
  s_event(Client *cl) : client(cl)
  {
  }
}               t_event;

class AuthSystem : public Observable
{
public:
  AuthSystem(World &world,
	     std::vector<Client *> &clients);
  virtual ~AuthSystem() = default;

  void  parseCmd(const void *data, int size, ENetPeer *peer);

private:
  void  handleConnection(const ClientMessage &message,
			 ENetPeer *peer);
  void	ping(const ClientMessage &message,
	     ENetPeer *peer);
  void	spawnClient(const std::vector<Client *> &clients,
		    Client *client);
  void	loadClientProfile(Client *client, const std::string &userId);
  void	generateNewId(std::string &guid);
  void	sendClientProfile(Client *client, const std::string &newId);
  void	sendSpawnInfo(Client *client) const;

  void    getPlayer(const ClientMessage &message,
		    ENetPeer *peer);

private:
  typedef void (AuthSystem::*parseFunc)(const ClientMessage &message,
					ENetPeer *peer);

  std::vector<Client *>	&_clients;
  std::map<ClientMessage::PacketContent, parseFunc> _func;
  World			&_world;
};

#endif /* _AUTHSYSTEM_H_ */
