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
    void createServer();
    void deleteServer();
    void getServer();
    
private:
    ENetHost            *_server;
    SQLite::Database    _db;
};

#endif
