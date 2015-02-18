#ifndef MASTERSERVER_HPP_
# define MASTERSERVER_HPP_

# include <iostream>
# include <enet/enet.h>
# include <SQLiteCpp/SQLiteCpp.h>

# include "MasterServerRequest.pb.h"
# include "MasterServerResponse.pb.h"

class MasterServer {
public:
  MasterServer();
  ~MasterServer();

  void	run();

private:
  void	createServer(ENetPeer *peer, const std::string &port,
		    const ServerData &info);
  void	deleteServer(ENetPeer *peer, const std::string &port);
  void	sendServers(ENetPeer *peer);
  void	sendServer(ENetPeer *peer, const ServerId &id);

private:
  ENetHost            *_server;
  SQLite::Database    _db;
};

#endif
