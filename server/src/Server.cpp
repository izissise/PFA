#include "server.hpp"

Server::Server(t_arg &arg)
: _arg(arg)
{
    if (enet_initialize() != 0)
        throw (NetworkException("An error occurred while initializing ENet."));

//    enet_address_set_host (&_address, "10.10.253.245");
    _address.host = ENET_HOST_ANY;
    _address.port = _arg.port;
    _server = enet_host_create(&_address, 128, 1, 10, 10);

    if (_server == NULL)
        throw (NetworkException("An error occurred while trying to create an ENet server host."));
}

Server::~Server()
{
    enet_host_destroy(_server);
    enet_deinitialize();
}

void Server::run()
{
    ENetEvent event;

    std::cout << "Actual port => " << _arg.port << std::endl;
    std::cout << "Quiet => " << _arg.quiet << std::endl;
    std::cout << "Debug => " << _arg.debug << std::endl;

    int val;

    while ((val = enet_host_service(_server, &event, 1000)) >= 0)
    {
        std::cout << "Val => " << val << std::endl;
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "A new client connected from " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
                event.peer->data = (char *)"Client information";
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                std::cout << "A packet of length " << event.packet->dataLength << "containing " << event.packet->data
                          << " was received from " << event.peer->data << " on channel " << event.channelID << std::endl;
                enet_packet_destroy (event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << event.peer->data << " disconnected." << std::endl;
                event.peer->data = NULL;
			case ENET_EVENT_TYPE_NONE:
            default:
                break;
        }
    }
}