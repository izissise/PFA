#ifndef Server_H_
#define Server_H_

#include <iostream>
#include <enet/enet.h>
#include "NetworkException.hpp"
#include "client.hpp"

#define DEFAULT_PORT 6060

typedef struct  s_arg
{
    bool        quiet;
    bool        debug;
    bool        ipv6;
    int         port;
    s_arg(): quiet(false), debug(false), port(DEFAULT_PORT) {};
}               t_arg;

class Server
{
public:
    Server(t_arg &arg);
    ~Server();

    void    run();

private:
    t_arg       _arg;
    ENetAddress _address;
    ENetHost*   _server;

};

#endif
