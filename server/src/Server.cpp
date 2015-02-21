#include <fstream>
#include <string>
#include "Server.hpp"
#include "CvarParser.hpp"
#include "printv.hpp"
#include "demangle.hpp"

Server::Server(ServerSettings &set)
  : _clients(),
    _masterSocket(),
    _set(set),
    _threadPool(200), je
    _world(_set),
    _proto(_world, _threadPool),
    _auth(_world, _clients)
{
  // enet_initialize is called in the _masterSocket()
  _address.host = ENET_HOST_ANY;
  _address.port = std::stoi(_set.getCvar("sv_port"));
  _server = enet_host_create(&_address, 128, 3, 0, 0);

  if (_server == NULL)
    throw (NetworkException("An error occurred while trying to create an ENet server host."));
  _set.addObserver(this);
  _auth.addObserver(this);

  try {
  registerToMaster();
  } catch (NetworkException& e) {
  	std::cerr << e.what() << std::endl;
  }
}

Server::~Server()
{
  MasterServerRequest   msg;
  std::string           packet;
  ENetEvent             event;
  int                   hasEvent;
  bool                  pull = true;
  bool                  passed = false;

  enet_host_destroy(_server);
  _masterSocket.connect(_set.getCvar("sv_masterIP"),
                        _set.getCvar("sv_masterPort"),
                        2);
  std::cout << "Shutting down server" << std::endl;
  while (!passed)
    {
      if ((hasEvent = _masterSocket.pullEvent(event, 50, pull)) > 0)
        {
          switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
              msg.set_content(MasterServerRequest::DELETESERVER);
              msg.set_port(_set.getCvar("sv_port"));
              msg.SerializeToString(&packet);
              _masterSocket.sendPacket(0, packet);
              enet_host_flush(_masterSocket.getHost());
              passed = true;
              break;
            default:
              break;
            }
        }
      else if (hasEvent < 0)
        break ;
    }
  _masterSocket.disconnect();
}

void    Server::registerToMaster()
{
  MasterServerRequest   msg;
  std::string           packet;
  ENetEvent             event;
  int                   hasEvent;
  bool                  pull = true;
  bool                  passed = false;

  _masterSocket.connect(_set.getCvar("sv_masterIP"),
                        _set.getCvar("sv_masterPort"),
                        2);
  while (!passed)
    {
      if ((hasEvent = _masterSocket.pullEvent(event, 50, pull)) > 0)
        {
          switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
              {
                ServerData      *data = new ServerData;

                msg.set_content(MasterServerRequest::CREATESERVER);
                msg.set_port(_set.getCvar("sv_port"));
                data->set_name(_set.getCvar("sv_hostname"));
                data->set_slots(std::stoul(_set.getCvar("sv_slot")));
                msg.set_allocated_info(data);
                msg.SerializeToString(&packet);
                _masterSocket.sendPacket(0, packet);
                enet_host_flush(_masterSocket.getHost());
                passed = true;
              }
              break;
            default:
              break;
            }
        }
      else if (hasEvent < 0)
        throw (NetworkException("Authentification to master server failed"));
    }
  _masterSocket.disconnect();
}

void            Server::run()
{
  ENetEvent     event;

  std::cout << "Actual port => " << _set.getCvar("sv_port") << std::endl;
  std::cout << "Quiet => " << _set.getCvar("sv_quiet") << std::endl;
  std::cout << "Debug => " << _set.getCvar("sv_debug") << std::endl;

  while ((enet_host_service(_server, &event, 50)) >= 0)
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

void    Server::handlePackets(ENetEvent &event)
{
  ENetPeer      *peer = event.peer;
  Client        *client = static_cast<Client *>(peer->data);
  ENetPacket    *packet = event.packet;

  // std::cout << "A packet of length "
  //        << packet->dataLength << " containing ["
  //        << packet->data
  //        << "] was received from "
  //        << peer->data
  //        << " user id -> "
  //        << peer->connectID
  //        << " on channel "
  //        << (int)event.channelID << std::endl;
  // if ((int)event.channelID == 2)
  //   std::cout << std::endl << std::endl;

  if (client == nullptr)
    _auth.parseCmd(packet->data, packet->dataLength, peer);
  else
    _proto.parseCmd(packet->data, packet->dataLength, client, _clients);
  enet_packet_destroy(packet);
}

void            Server::connectClient(ENetPeer * const peer)
{
  std::cout << "A new client connected from "
            << peer->address.host << " : "
            << peer->address.port << std::endl;
  peer->data = nullptr;
}

void    Server::disconnectClient(ENetPeer * const peer)
{
  std::cout << "Client disconnected" << std::endl;
  if (peer->data == nullptr) // not authenticated player
    return ;
  saveClientId(static_cast<Client *>(peer->data));
  peer->data = NULL;
  _clients.erase(std::find_if(_clients.begin(), _clients.end(),
                              [peer] (Client * const elem)
                              { return elem->getPeer() == peer;}));
}

void    Server::updateClients()
{
  for (auto client : _clients)
    {
      const std::vector<bool>   &actions = client->getActions();
      unsigned int              vecSize = actions.size();

      for (unsigned int actId = 0; actId < vecSize; ++actId)
        {
          Action        act = static_cast<Action>(actId);

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

void    Server::saveClientId(Client *client)
{
  const ClientInfo      &clInfo = client->getInfo();
  const ClientEntity    &clEnt = client->getEntity();
  const std::string     &clId = clInfo.getId();
  const Vector2f        &position = clEnt.getPosition();
  const Vector2i        &chunkId = clEnt.getChunkId();

  std::fstream          file;
  std::ostringstream    newLine("");
  std::vector<std::string> content;
  std::string           line;
  bool                  found = false;
  unsigned int          lineIdx = 0;

  file.open(LOGFILE, std::ios::binary | std::ios::in);
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
  while (getline(file, line))
    {
      content.push_back(line);
      if (!found && line.find(clId) != std::string::npos)
        found = true;
      else if (!found)
        ++lineIdx;
    }
  if (found)
    content[lineIdx] = newLine.str();
  else
    content.push_back(newLine.str());
  file.close();
  file.open(LOGFILE, std::ios::binary | std::ios::out);
  if (!file)
    throw std::invalid_argument(std::string("Cannot acces to the file ") + LOGFILE);
  for (auto it : content)
    file << it << std::endl;
  file.close();
}

void            Server::actDisplacement(Client *client, Action act)
{
  ClientEntity  &clEnt = client->getEntity();
  Vector2f      ratio = {CHUNKWIDTH / 256.f, CHUNKHEIGHT / 256.f};
  // to remove, let me maintain the speed

  clEnt.move(Vector2f(act == Action::Left ? -0.01 / ratio.x :
                      act == Action::Right ? 0.01 / ratio.x :
                      0,
                      act == Action::Forward ? 0.01 / ratio.y :
                      act == Action::Back ? -0.01 / ratio.y :
                      0));


  // petite partie en dur :D
  const Vector2f        &plPos = clEnt.getPosition();
  const Vector2i        &plChunk = clEnt.getChunkId();

  ProtocolMessage       msg;
  Displacement          *displacement = new Displacement;
  VectorFloat           *acceleration = new VectorFloat;
  VectorFloat           *velocity = new VectorFloat;
  Position              *position = new Position;
  VectorInt             *chunkId = new VectorInt;
  VectorFloat           *pos = new VectorFloat;
  std::string           serialized;

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

void            Server::trigger(const t_event &event)
{
  Client        *client = event.client;

  _clients.push_back(client);
  client->sendPacket(0, _set.serialize());
  std::cout << "Client authenticated" << std::endl;
}
