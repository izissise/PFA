#ifndef Client_H_
#define Client_H_

#include <iostream>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

class Client : public boost::enable_shared_from_this<Client>
{
public:
    typedef boost::shared_ptr<Client> pointer;
    
    static pointer  create(boost::asio::io_service& io_service);
    tcp::socket&    socket();
    void            start();
    void handle_write(const boost::system::error_code& /*error*/,
                      size_t /*bytes_transferred*/);
    
private:
    Client(boost::asio::io_service &io_service);

    tcp::socket _socket;
};

#endif
