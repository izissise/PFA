#ifndef MASTERSERVER_HPP_
# define MASTERSERVER_HPP_

# include <iostream>
# include <enet/enet.h>
# include <SQLiteCpp/SQLiteCpp.h>

# include "MasterServerRequest.pb.h"
# include "MasterServerResponse.pb.h"
# include "ClientMessage.pb.h"
# include "ServerResponse.pb.h"

class MasterServer {
public:
  MasterServer();
  ~MasterServer();

  void run();

private:

  void createServer(ENetPeer *peer, const std::string &port,
                    const ServerData &info);
  void deleteServer(ENetPeer *peer, const std::string &port);
  void getServer(const char *ip, int port);

  void sendServers(ENetPeer *peer);
  void sendServer(ENetPeer *peer, const ServerId &id);

  int getServerPlayer(const char *ip, const char *port) const;

  void parsePacket(ENetPacket *packet, ENetPeer *peer);
  void getServerInfo(SQLite::Statement &query, ServerInfo *server);

  void sendPacket(ENetPeer *peer, const ::google::protobuf::Message &message) const;

  ENetHost            *_server;
  SQLite::Database    _db;
};

#endif
