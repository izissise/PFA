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

class Server: public IObserver
{
public:
  Server(ServerSettings &set);
  ~Server();

  void          run();
  virtual void  trigger(const t_event &event);

private:
  void  handlePackets(ENetEvent &event);
  void  connectClient(ENetPeer * const peer);
  void  disconnectClient(ENetPeer * const peer);
  void  updateClients();
  void  saveClientId(Client *cl);
  void  actDisplacement(Client *client, Action act);
  void  registerToMaster();

private:
  std::vector<Client *> _clients;
  Network               _masterSocket;
  ServerSettings        &_set;
  ThreadPool            _threadPool;
  ENetAddress           _address;
  ENetHost*             _server;
  World                 _world;
  ServerProtocol        _proto;
  AuthSystem            _auth;
  bool			_masterAuthenticate;
};

#endif
