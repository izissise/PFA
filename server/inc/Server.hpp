#ifndef Server_H_
# define Server_H_

# include <enet/enet.h>
# include <vector>
# include <algorithm>
# include <iostream>
# include "NetworkException.hpp"
# include "Client.hpp"
# include "Observer.hpp"
# include "ServerSettings.hpp"
# include "ServerProtocol.hpp"

#define DEFAULT_PORT 6060

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
  void	handlePackets(ServerProtocol &proto,
		      ENetEvent &event);
  void	connectClient(ENetPeer * const client);
  void	disconnectClient(ENetPeer * const client);

private:
  t_arg               _arg;
  ServerSettings      _set;
  ENetAddress         _address;
  ENetHost*           _server;
  std::vector<Client *>	_clients;
};

#endif
