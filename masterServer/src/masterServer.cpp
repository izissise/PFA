#include <thread>
#include <Network.hpp>
#include "masterServer.hpp"
#include "IpToCountrySlow.hpp"

MasterServer::MasterServer()
  : _db("server.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE)
{
  if (enet_initialize () != 0)
    throw NetworkException("An error occurred while initializing ENet.");

  ENetAddress address;

  address.host = ENET_HOST_ANY;
  address.port = 25255;
  _server = enet_host_create (& address /* the address to bind the server host to */,
                              32      /* allow up to 32 clients and/or outgoing connections */,
                              2      /* allow up to 2 channels to be used, 0 and 1 */,
                              0      /* assume any amount of incoming bandwidth */,
                              0      /* assume any amount of outgoing bandwidth */);
  if (_server == NULL)
    throw NetworkException("An error occurred while trying to create an ENet server host.");
  std::cout << "Server start on " << address.port << std::endl;

  try
    {
      _db.exec("CREATE TABLE IF NOT EXISTS server"
               "(ip TEXT, port TEXT, name TEXT, currentPlayer INTEGER, slots INTEGER, country TEXT, lastUpdate INTEGER, "
               "PRIMARY KEY (ip, port))");
    }
  catch (std::exception& e)
    {
      std::cerr << "exception: " << e.what() << std::endl;
    }
}

MasterServer::~MasterServer()
{
  if (_server)
    enet_host_destroy(_server);
  enet_deinitialize();
}

void MasterServer::createServer(ENetPeer *peer, const std::string &port,
                                const ServerData &info)
{
  char ip[256] = { 0 };

  enet_address_get_host_ip(&peer->address, ip, 256);
  try
    {
      std::string c;
      try {
        IpToCountrySlow iptc;
        IpAddressMapping country = iptc.GetCountry(ip);
        c = country.country;
      } catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << std::endl;
        c = "N/A";
      }
      SQLite::Statement st(_db, "INSERT INTO server VALUES (?, ?, ?, 0, ?, ?, ?)");

      st.bind(1, ip);
      st.bind(2, port);
      st.bind(3, info.name());
      st.bind(4, static_cast<int>(info.slots()));
      st.bind(5, c);
      st.bind(6, 0);
      int nb = st.exec();

      std::cout << st.getQuery() << ", returned " << nb << std::endl;
    }
  catch (std::exception& e)
    {
      std::cerr << "exception: " << e.what() << std::endl;
    }
}

void MasterServer::getServerInfo(SQLite::Statement &query, ServerInfo *server)
{
  int laps = 2 * 60; // 2 minutes

  // Demonstrate how to get some typed column value
  const char* ip = query.getColumn(0).getText("N/A");
  const char* port = query.getColumn(1).getText("N/A");
  const char* name = query.getColumn(2).getText("N/A");
  unsigned int currentPlayer = query.getColumn(3).getInt();
  unsigned int slots = query.getColumn(4).getInt();
  const char *country = query.getColumn(5).getText("N/A");

  std::cout << "row: " << ip << ", " << port << std::endl;
  if (query.getColumn(6).getInt() + laps < std::time(NULL))
    {
      try {
        currentPlayer = getServerPlayer(ip, port);
      } catch (NetworkException &e) {
        std::cerr << e.what() << std::endl;
        currentPlayer = 0;
      }

      SQLite::Statement st(_db, "UPDATE server SET currentPlayer = ?, lastUpdate = ? WHERE ip = ? AND port = ?");

      st.bind(1, static_cast<int>(currentPlayer));
      st.bind(2, static_cast<int>(std::time(NULL)));
      st.bind(3, ip);
      st.bind(4, port);
      st.exec();

    }

  server->set_ip(ip);
  server->set_port(port);
  server->set_name(name);
  server->set_currentplayer(currentPlayer);
  server->set_maxplayer(slots);
  server->set_country(country);

}

int MasterServer::getServerPlayer(const char *ip, const char *port) const
{
  Network client;
  int currentPlayer;
  ENetEvent event;
  bool first = true;
  int i = 0;

  client.connect(ip, port, 1);
  while (client.pullEvent(event, 1000, first) && i < 3)
    {
      switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
          {
            ClientMessage request;
            std::string message;

            request.set_content(ClientMessage::GETPLAYER);
            request.SerializeToString(&message);
            client.sendPacket(0, message);
            break;
          }
        case ENET_EVENT_TYPE_RECEIVE:
          {
            ServerResponse response;

            if (response.ParseFromArray(event.packet->data, event.packet->dataLength)
                && response.content() == ServerResponse::PLAYER)
              {
                currentPlayer = response.player();
                client.disconnect();
                return currentPlayer;
              }
            else
              currentPlayer = -1;
            break;
          }
        default:
          break;
        }
      ++i;
    }
  if (i == 3)
    throw NetworkException(std::string("No Response Received from ") + ip + ":" + port);
  return currentPlayer;
}

void MasterServer::deleteServer(ENetPeer *peer, const std::string &port)
{
  char name[256] = { 0 };

  enet_address_get_host_ip(&peer->address, name, 256);
  try
    {
      SQLite::Statement st(_db, "DELETE FROM server WHERE ip LIKE ? AND port LIKE ?");

      st.bind(1, name);
      st.bind(2, port);
      int nb = st.exec();

      std::cout << st.getQuery() << ", returned " << nb << std::endl;
    }
  catch (std::exception& e)
    {
      std::cerr << "exception: " << e.what() << std::endl;
    }
}

void MasterServer::sendServers(ENetPeer *peer)
{
  try
    {
      SQLite::Statement   query(_db, "SELECT * FROM server");

      // Loop to execute the query step by step, to get rows of result
      while (query.executeStep())
        {
          MasterServerResponse response;
          ServerInfo *server = new ServerInfo();

          getServerInfo(query, server);

          response.set_content(MasterServerResponse::IP);
          response.set_allocated_server(server);
          response.set_place(0);

          sendPacket(peer, response);
        }
    }
  catch (std::exception& e)
    {
      std::cerr << "exception: " << e.what() << std::endl;
    }
}

void    MasterServer::sendServer(ENetPeer *peer, const ServerId &id)
{
  try
    {
      SQLite::Statement   query(_db, "SELECT * from server WHERE ip LIKE ? AND port LIKE ?");

      query.bind(1, id.ip());
      query.bind(2, id.port());
      // Loop to execute the query step by step, to get rows of result
      if (query.executeStep())
        {
          MasterServerResponse response;
          ServerInfo *server = new ServerInfo();

          getServerInfo(query, server);

          response.set_content(MasterServerResponse::IP);
          response.set_allocated_server(server);
          response.set_place(1);

          sendPacket(peer, response);
        }
    }
  catch (std::exception& e)
    {
      std::cerr << "exception: " << e.what() << std::endl;
    }
}


void MasterServer::parsePacket(ENetPacket *packet, ENetPeer *peer)
{
  MasterServerRequest request;
  if (request.ParseFromArray(packet->data, packet->dataLength))
    {
      switch (request.content())
        {
        case MasterServerRequest::GETSERVERS:
          sendServers(peer);
          break;

        case MasterServerRequest::GETIP:
          sendServer(peer, request.id());
          break;

        case MasterServerRequest::CREATESERVER:
          createServer(peer, request.port(), request.info());
          break;

        case MasterServerRequest::DELETESERVER:
          deleteServer(peer, request.port());
          break;

        default:
          break;
        }

    }

  /* Clean up the packet now that we're done using it. */
  enet_packet_destroy(packet);
}

void MasterServer::sendPacket(ENetPeer *peer, const ::google::protobuf::Message &message) const
{
  std::string msg;
  message.SerializeToString(&msg);

  ENetPacket *packet = enet_packet_create(msg.c_str(), msg.size(),
                                          ENET_PACKET_FLAG_RELIABLE);
  if (packet == nullptr || enet_peer_send(peer, 0, packet) != 0)
    std::cerr << "Cannot be send"<< std::endl;
}

void MasterServer::run()
{
  ENetEvent event;
  while ((enet_host_service (_server, &event, 50)) >= 0)
    {
      switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
          {
            char name[256] = { 0 };
            enet_address_get_host_ip(&event.peer->address, name, 256);
            std::cout << "A new client connected from " << name << ":" << event.peer->address.port << std::endl;
            break;
          }
        case ENET_EVENT_TYPE_RECEIVE:
          {
            std::thread t(&MasterServer::parsePacket, this, event.packet, event.peer);
            t.detach();
            break;
          }
        default:
          break;
        }
    }
}
