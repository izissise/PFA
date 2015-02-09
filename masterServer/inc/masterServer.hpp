#ifndef MASTERSERVER_HPP_
# define MASTERSERVER_HPP_

# include <iostream>
# include <enet/enet.h>

class MasterServer {
public:
  MasterServer();
  ~MasterServer();

  void run();
  
private:
  ENetHost  *_server;
};

#endif
