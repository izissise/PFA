#ifndef MASTERSERVER_HPP_
# define MASTERSERVER_HPP_

# include <iostream>
# include <enet/enet.h>
# include <SQLiteCpp/SQLiteCpp.h>

class MasterServer {
public:
    MasterServer();
    ~MasterServer();
    
    void run();
    void createServer(ENetPeer *peer, const std::string &port);
    void deleteServer(ENetPeer *peer, const std::string &port);
    void getServer(ENetPeer *peer);
    
    void parsePacket(ENetPacket *packet, ENetPeer *peer);
    
private:
    ENetHost            *_server;
    SQLite::Database    _db;
};

#endif
