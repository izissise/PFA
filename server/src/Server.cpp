#include <fstream>
#include <string>
#include "Server.hpp"
#include "CvarParser.hpp"
#include "printv.hpp"
#include "demangle.hpp"

Server::Server(t_arg &arg)
  : _arg(arg), _set(),
    _world(_set), _proto(_world), _clients()
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
	  handlePackets(event);
	  break;
	case ENET_EVENT_TYPE_DISCONNECT:
	  disconnectClient(event.peer);
	default:
	  break;
        }
    }
}

void	Server::handlePackets(ENetEvent &event)
{
  ENetPeer	*peer = event.peer;
  Client	*client = static_cast<Client *>(peer->data);
  ENetPacket	*packet = event.packet;

  std::cout << "A packet of length "
	    << packet->dataLength << " containing ["
	    << packet->data
	    << "] was received from "
	    << peer->data
	    << " user id -> "
	    << peer->connectID
	    << " on channel "
	    << (int)event.channelID << std::endl;
  _proto.parseCmd(packet->data, packet->dataLength, client, _clients);
  enet_packet_destroy(packet);
}

void		Server::connectClient(ENetPeer * const peer)
{
  Client	*newClient = new Client(peer);

  std::cout << "A new client connected from "
	    << peer->address.host << " : "
	    << peer->address.port << std::endl;
  peer->data = newClient;
  newClient->getEntity().setPosition({128, 128});
  newClient->getEntity().setChunkId({0, 0});
  _clients.push_back(newClient);
  newClient->sendPacket(0, _set.serialize());
}

void	Server::disconnectClient(ENetPeer * const peer)
{
  std::cout << "Client disconnected" << std::endl;
  saveClientId(static_cast<Client *>(peer->data));
  peer->data = NULL;
  _clients.erase(std::find_if(_clients.begin(), _clients.end(),
			      [peer] (Client * const elem)
			      { return elem->getPeer() == peer;}));
}

void	Server::saveClientId(Client *client)
{
  const ClientInfo	&clInfo = client->getInfo();
  const ClientEntity	&clEnt = client->getEntity();
  const std::string	&clId = clInfo.getId();
  const Vector2f	&position = clEnt.getPosition();
  const Vector2i	&chunkId = clEnt.getChunkId();

  std::fstream		file;
  std::vector<std::string> content;
  std::ostringstream	newLine("");
  unsigned int		foundId = 0;
  bool			found = false;

  file.open(LOGFILE, std::fstream::binary | std::fstream::in | std::fstream::out);
  printv(newLine, "%,% %,% %",
	  clId, chunkId.x, chunkId.y,
	  position.x, position.y);
  if (!file)
    {
      file.open(LOGFILE, std::fstream::binary | std::fstream::out);
      if (!file)
	throw std::invalid_argument(std::string(LOGFILE) + ": File not found");
      file << newLine.str() << std::endl;
      file.close();
      return ;
    }
  for (std::string line; getline(file, line);)
    {
      content.push_back(line);
      if (!found && line.find(clId) != std::string::npos)
	found = true;
      if (!found)
	++foundId;
    }
  if (found)
    content[foundId] = newLine.str();
  else
    content.push_back(newLine.str());
  for (auto &s : content)
    file << s << std::endl;
  file.close();
}

void	Server::trigger(const t_event &event)
{
  std::cout << event.change << " Change" << std::endl;
}
