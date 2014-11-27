#include "Server.hpp"

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
  ENetEvent       event;
  ServerProtocol  proto;

  std::cout << "Actual port => " << _arg.port << std::endl;
  std::cout << "Quiet => " << _arg.quiet << std::endl;
  std::cout << "Debug => " << _arg.debug << std::endl;

  while ((enet_host_service(_server, &event, 1000)) >= 0)
    {
      switch (event.type)
        {
	case ENET_EVENT_TYPE_CONNECT:
	  connectClient(event.peer);
	  break;
	case ENET_EVENT_TYPE_RECEIVE:
	  handlePackets(proto, event);
	  break;
	case ENET_EVENT_TYPE_DISCONNECT:
	  disconnectClient(event.peer);
	default:
	  break;
        }
    }
}

void	Server::handlePackets(ServerProtocol &proto,
			      ENetEvent &event)
{
  ENetPeer	*client = event.peer;
  ENetPacket	*packet = event.packet;

  for (auto it : _clients)
    it->sendPacket(0, std::string(reinterpret_cast<char *>(packet->data)));
  std::cout << "A packet of length "
	    << packet->dataLength << " containing ["
	    << packet->data
	    << "] was received from "
	    << client->data
	    << " user id -> "
	    << client->connectID
	    << " on channel "
	    << (int)event.channelID << std::endl;
  proto.parseCmd(packet->data, packet->dataLength);
  enet_packet_destroy(packet);
}

void	Server::connectClient(ENetPeer * const client)
{
  std::cout << "A new client connected from "
	    << client->address.host << " : "
	    << client->address.port << std::endl;
  client->data = const_cast<char *>("Client");

  _clients.push_back(new Client(client));
}

void	Server::disconnectClient(ENetPeer * const client)
{
  std::cout << "Client disconnected" << std::endl;
  client->data = NULL;
  _clients.erase(std::find_if(_clients.begin(), _clients.end(),
			      [client] (const Client *elem)
			      { return elem->getPeer() == client;}));
}

void	Server::trigger(const t_event &event)
{
  std::cout << event.change << " Change" << std::endl;
}
