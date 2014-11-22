#ifndef Server_H_
# define Server_H_

# include <iostream>
# include <list>
# include <algorithm>
# include <enet/enet.h>
# include "NetworkException.hpp"
# include "client.hpp"
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
    s_arg(): quiet(false), debug(false), port(DEFAULT_PORT) {};
}               t_arg;

class Server: public IObserver
{
public:
    Server(t_arg &arg);
    ~Server();

    void          run();
    virtual void	trigger(const t_event &event);

private:
    t_arg               _arg;
    ServerSettings      _set;
    ENetAddress         _address;
    ENetHost*           _server;
    std::list<Client *> _clients;
};

#endif
