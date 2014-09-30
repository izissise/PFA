#include "server.hpp"

Server::Server(t_arg &arg)
: _arg(arg), _set(), _clients()
{
    if (enet_initialize() != 0)
        throw (NetworkException("An error occurred while initializing ENet."));

    _address.host = ENET_HOST_ANY;
    _address.port = _arg.port;
    _server = enet_host_create(&_address, 128, 2, 10, 10);

    if (_server == NULL)
        throw (NetworkException("An error occurred while trying to create an ENet server host."));
    _set.addObserver(this);
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

    while ((enet_host_service(_server, &event, 1000)) >= 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "A new client connected from " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
                _clients.push_back(new Client(event.peer));
                event.peer->data = (char *)"Client";
                break;
            case ENET_EVENT_TYPE_RECEIVE:
              for (auto it : _clients)
                it->sendPacket(0, std::string(reinterpret_cast<char *>(event.packet->data)));
              std::cout << "A packet of length " << event.packet->dataLength << " containing [" << event.packet->data
                        << "] was received from " << event.peer->data << " on channel " << (int)event.channelID << std::endl;
              enet_packet_destroy (event.packet);
              break;
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Client disconnected" << std::endl;
                _clients.erase(std::find_if(_clients.begin(), _clients.end(),
                                           [event] (const Client *elem) { return elem->getPeer() == event.peer;}));
                event.peer->data = NULL;
            default:
                break;
        }
      _set.setCvar("com_displayFps", "60");
    }
}

void  Server::trigger(const t_event &event)
{
  std::cout << event.change << " Change" << std::endl;
}
