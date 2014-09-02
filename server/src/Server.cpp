#include <boost/lexical_cast.hpp>
#include "Server.h"

using boost::asio::ip::tcp;

Server::Server(t_arg &arg, boost::asio::io_service &io_service)
: _arg(arg), _acceptor(io_service, tcp::endpoint((arg.ipv6 ? tcp::v6() : tcp::v4()) , arg.port))
{
    start_accept();
}

void Server::start_accept()
{
    Client::pointer new_connection =
    Client::create(_acceptor.get_io_service());
    
    _acceptor.async_accept(new_connection->socket(),
                           boost::bind(&Server::handle_accept, this, new_connection,
                                       boost::asio::placeholders::error));
}

void Server::handle_accept(Client::pointer connect,
                           const boost::system::error_code& error)
{
    if (!error)
    {
        connect->start();
        start_accept();
    }
    else
        throw (boost::system::system_error(error));
}