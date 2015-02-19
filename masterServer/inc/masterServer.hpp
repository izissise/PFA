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
    void createServer(ENetPeer *peer, const std::string &port,
                      const ServerData &info);
    void deleteServer(ENetPeer *peer, const std::string &port);
    void getServer(ENetPeer *peer);
    
    int getServerPlayer(const char *ip, int port) const;
    
    void parsePacket(ENetPacket *packet, ENetPeer *peer);
    void getServerInfo(SQLite::Statement &query, ServerInfo *server);
    
private:
    ENetHost            *_server;
    SQLite::Database    _db;
};

#endif
