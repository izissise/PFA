#include "client.h"

Client::Client(boost::asio::io_service &io_service)
: _socket(io_service)
{
    
}

Client::pointer Client::create(boost::asio::io_service& io_service)
{
    return pointer(new Client(io_service));
}

tcp::socket& Client::socket()
{
    return _socket;
}

void Client::handle_write(const boost::system::error_code& /*error*/,
                  size_t /*bytes_transferred*/)
{
    std::cout << "Write" << std::endl;
}

void Client::start()
{
    boost::asio::async_write(_socket, boost::asio::buffer("Hello World!"),
                             boost::bind(&Client::handle_write, shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}
