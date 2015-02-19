#ifndef Server_H_
# define Server_H_

# include <enet/enet.h>
# include <algorithm>
# include <iostream>
# include <functional>

# include "Network.hpp"
# include "Observer.hpp"
# include "ServerSettings.hpp"
# include "ServerProtocol.hpp"
# include "ThreadPool.hpp"
# include "AuthSystem.hpp"
# include "MasterServerRequest.pb.h"

# define DEFAULT_PORT 6000

typedef struct  s_arg
{
  bool        quiet;
  bool        debug;
  bool        ipv6;
  int         port;
  s_arg():
    quiet(false), debug(false), port(DEFAULT_PORT)
  {
  };
}               t_arg;

class Server: public IObserver
{
public:
  Server(t_arg &arg);
  ~Server();

  void		run();
  virtual void	trigger(const t_event &event);

private:
  void	handlePackets(ENetEvent &event);
  void	connectClient(ENetPeer * const peer);
  void	disconnectClient(ENetPeer * const peer);
  void	updateClients();
  void	saveClientId(Client *cl);
  void	actDisplacement(Client *client, Action act);
  void	registerToMaster();

private:
  t_arg			_arg;
  std::vector<Client *>	_clients;
  Network		_masterSocket;
  ServerSettings	_set;
  ThreadPool		_threadPool;
  ENetAddress		_address;
  ENetHost*		_server;
  World			_world;
  ServerProtocol	_proto;
  AuthSystem		_auth;
};

#endif
