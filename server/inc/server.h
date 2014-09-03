#ifndef Server_H_
#define Server_H_

#include <iostream>
#include <boost/asio.hpp>
#include "client.h"

#define DEFAULT_PORT 6060

typedef struct  s_arg
{
    bool        quiet;
    bool        debug;
    bool        ipv6;
    int         port;
    s_arg(): quiet(false), debug(false), port(DEFAULT_PORT) {};
}               t_arg;

using boost::asio::ip::tcp;

class Server
{
public:
    Server(t_arg &arg, boost::asio::io_service &io_service);

private:
    void    start_accept();
    void    handle_accept(Client::pointer new_connection,
                          const boost::system::error_code& error);

    t_arg                   _arg;
    boost::asio::io_service _io_service;
    tcp::acceptor           _acceptor;
};

#endif
