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

void		Server::run()
{
  ENetEvent	event;

  std::cout << "Actual port => " << _arg.port << std::endl;
  std::cout << "Quiet => " << _arg.quiet << std::endl;
  std::cout << "Debug => " << _arg.debug << std::endl;

  while ((enet_host_service(_server, &event, 17)) >= 0)
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
      updateClients();
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

void	Server::updateClients()
{
  for (auto client : _clients)
    {
      const std::vector<bool>	&actions = client->getActions();
      unsigned int		vecSize = actions.size();

      for (unsigned int actId = 0; actId < vecSize; ++actId)
	{
	  Action	act = static_cast<Action>(actId);

	  if (actions[actId] == true)
	    {
	      switch (act)
		{
		case Action::Forward:
		case Action::Back:
		case Action::Right:
		case Action::Left:
		  actDisplacement(client, act);
		  break;
		default:
		  std::cout << "Action n: " << actId << " not implemented" << std::endl;
		  break;
		}
	    }
	}
    }
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

  file.open(LOGFILE, std::ios::binary | std::ios::in | std::ios::out);
  printv(newLine, "%;% %;% %",
	  clId, chunkId.x, chunkId.y,
	  position.x, position.y);
  if (!file)
    {
      file.open(LOGFILE, std::ios::binary | std::ios::out);
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
    {
      std::cout << "write to file " << s << std::endl;
      file << s << std::endl;
    }
  file.close();
}

void		Server::actDisplacement(Client *client, Action act)
{
  ClientEntity	&clEnt = client->getEntity();

  clEnt.move(Vector2f(act == Action::Left ? -0.01 : act == Action::Right ? 0.01 : 0,
		      act == Action::Forward ? 0.01 : act == Action::Back ? -0.01 : 0));


  // petite partie en dur :D
  const Vector2f	&plPos = clEnt.getPosition();
  const Vector2i	&plChunk = clEnt.getChunkId();

  ProtocolMessage       msg;
  Displacement		*displacement = new Displacement;
  VectorFloat		*acceleration = new VectorFloat;
  VectorFloat		*velocity = new VectorFloat;
  Position		*position = new Position;
  VectorInt		*chunkId = new VectorInt;
  VectorFloat		*pos = new VectorFloat;
  std::string		serialized;

  // quick hard coded value
  acceleration->set_x(0);
  acceleration->set_y(0);
  velocity->set_x(0);
  velocity->set_y(0);

  chunkId->set_x(plChunk.x);
  chunkId->set_y(plChunk.y);
  pos->set_x(plPos.x);
  pos->set_y(plPos.y);
  position->set_allocated_chunkid(chunkId);
  position->set_allocated_pos(pos);

  displacement->set_allocated_acceleration(acceleration);
  displacement->set_allocated_velocity(velocity);
  displacement->set_allocated_position(position);

  msg.set_content(ProtocolMessage::DISPLACEMENT);
  msg.set_allocated_displacement(displacement);
  msg.SerializeToString(&serialized);
  client->sendPacket(0, serialized);
}

void	Server::trigger(const t_event &event)
{
  std::cout << event.change << " Change" << std::endl;
}
