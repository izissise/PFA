#include "Server.hpp"
#include "CvarParser.hpp"

Server::Server(t_arg &arg)
  : _arg(arg), _set(), _world(_set), _clients()
{
  if (enet_initialize() != 0)
    throw (NetworkException("An error occurred while initializing ENet."));

  _address.host = ENET_HOST_ANY;
  _address.port = _arg.port;
  _server = enet_host_create(&_address, 128, 2, 10, 10);

  if (_server == NULL)
    throw (NetworkException("An error occurred while trying to create an ENet server host."));
  _set.addObserver(this);

  CvarParser	parser(_set);

  parser.loadConfigFile("../ServerConfig.cfg");
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
  ENetPeer	*peer = event.peer;
  Client	*client = static_cast<Client *>(peer->data);
  ENetPacket	*packet = event.packet;

  for (auto it : _clients)
    it->sendPacket(0, std::string(reinterpret_cast<char *>(packet->data)));
  std::cout << "A packet of length "
	    << packet->dataLength << " containing ["
	    << packet->data
	    << "] was received from "
	    << peer->data
	    << " user id -> "
	    << peer->connectID
	    << " on channel "
	    << (int)event.channelID << std::endl;
  proto.parseCmd(packet->data, packet->dataLength);
  enet_packet_destroy(packet);
}

void		Server::connectClient(ENetPeer * const peer)
{
  Client	*newClient = new Client(peer);

  std::cout << "A new client connected from "
	    << peer->address.host << " : "
	    << peer->address.port << std::endl;
  peer->data = newClient;
  _clients.push_back(newClient);

  newClient->sendPacket(0, _set.serialize());
  std::vector<Vector2i> chunks;
  for (int y = -1; y <= 1; ++y)
    {
      for (int x = -1; x <= 1; ++x)
	{
	  _world.loadChunk(_clients, x, y);
	  chunks.push_back({x, y});
	}
    }
  newClient->sendPacket(0, _world.serialize(chunks));
}

void	Server::disconnectClient(ENetPeer * const peer)
{
  std::cout << "Client disconnected" << std::endl;
  peer->data = NULL;
  _clients.erase(std::find_if(_clients.begin(), _clients.end(),
			      [peer] (const Client *elem)
			      { return elem->getPeer() == peer;}));
}

void	Server::trigger(const t_event &event)
{
  std::cout << event.change << " Change" << std::endl;
}
