#include "masterServer.hpp"

MasterServer::MasterServer()
: _db("server.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE)
{
    if (enet_initialize () != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        exit (EXIT_FAILURE);
    }

    ENetAddress address;

    address.host = ENET_HOST_ANY;
    address.port = 25255;
    _server = enet_host_create (& address /* the address to bind the server host to */,
                                32      /* allow up to 32 clients and/or outgoing connections */,
                                2      /* allow up to 2 channels to be used, 0 and 1 */,
                                0      /* assume any amount of incoming bandwidth */,
                                0      /* assume any amount of outgoing bandwidth */);
    if (_server == NULL)
    {
        std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
        exit (EXIT_FAILURE);
    }
    std::cout << "Server start on " << address.port << std::endl;

    try
    {
      _db.exec("CREATE TABLE IF NOT EXISTS server"
	       "(ip TEXT, port TEXT, name TEXT, slots INTEGER,"
	       "PRIMARY KEY (ip, port))");
    }
    catch (std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
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
      SQLite::Statement st(_db, "INSERT INTO server VALUES (?, ?, ?, ?)");

      st.bind(1, ip);
      st.bind(2, port);
      st.bind(3, info.name());
      st.bind(4, static_cast<int>(info.slots()));
      int nb = st.exec();

      std::cout << st.getQuery() << ", returned " << nb << std::endl;
    }
  catch (std::exception& e)
    {
      std::cout << "exception: " << e.what() << std::endl;
    }
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
        std::cout << "exception: " << e.what() << std::endl;
    }
    std::cout << "Delete" << std::endl;
}

void MasterServer::sendServers(ENetPeer *peer)
{
  try
    {
      SQLite::Statement   query(_db, "SELECT * FROM server");

      while (query.executeStep())
	{
	  MasterServerResponse response;
	  std::string message;
	  ServerInfo *server = new ServerInfo();
	  const char* ip = query.getColumn(0).getText("N/A");
	  const char* port = query.getColumn(1).getText("N/A");
	  const char* name = query.getColumn(2).getText("N/A");
	  unsigned int slots = query.getColumn(3).getInt();

	  server->set_ip(ip);
	  server->set_port(port);
	  server->set_name(name);
	  server->set_currentplayer(0);
	  server->set_maxplayer(slots);
	  server->set_country("FR");

	  response.set_content(MasterServerResponse::IP);
	  response.set_allocated_server(server);
	  response.SerializeToString(&message);

	  ENetPacket *packet = enet_packet_create(message.c_str(), message.size(),
                                                    ENET_PACKET_FLAG_RELIABLE);
	  if (packet == nullptr || enet_peer_send(peer, 0, packet) != 0)
	    {
	      std::cerr << "row: " << ip << ", " << port << " Cannot be send"<< std::endl;
	    }
	  std::cout << "row: " << ip << ", " << port << std::endl;
        }
    }
  catch (std::exception& e)
    {
      std::cout << "exception: " << e.what() << std::endl;
    }
}

void	MasterServer::sendServer(ENetPeer *peer, const ServerId &id)
{
  try
    {
      SQLite::Statement   query(_db, "SELECT * from server WHERE ip LIKE ? AND port LIKE ?");

      query.bind(1, id.ip());
      query.bind(2, id.port());
      while (query.executeStep())
	{
	  MasterServerResponse response;
	  std::string message;
	  ServerInfo *server = new ServerInfo();
	  const char* ip = query.getColumn(0).getText("N/A");
	  const char* port = query.getColumn(1).getText("N/A");
	  const char* name = query.getColumn(2).getText("N/A");
	  unsigned int slots = query.getColumn(3).getInt();

	  server->set_ip(ip);
	  server->set_port(port);
	  server->set_name(name);
	  server->set_currentplayer(0);
	  server->set_maxplayer(slots);
	  server->set_country("FR");

	  response.set_content(MasterServerResponse::IP);
	  response.set_allocated_server(server);
	  response.SerializeToString(&message);

	  ENetPacket *packet = enet_packet_create(message.c_str(), message.size(),
						  ENET_PACKET_FLAG_RELIABLE);
	  if (packet == nullptr || enet_peer_send(peer, 0, packet) != 0)
	    {
	      std::cerr << "row: " << ip << ", " << port << " Cannot be send"<< std::endl;
	    }
	  std::cout << "row: " << ip << ", " << port << std::endl;
        }
    }
  catch (std::exception& e)
    {
      std::cout << "exception: " << e.what() << std::endl;
    }
}

void MasterServer::run()
{
  ENetEvent event;
  MasterServerRequest request;

  while ((enet_host_service (_server, &event, 50)) >= 0)
    {
      switch (event.type)
        {
	case ENET_EVENT_TYPE_CONNECT:
	  printf ("A new client connected from %x:%u.\n",
		  event.peer -> address.host,
		  event.peer -> address.port);
	  /* Store any relevant client information here. */
	  event.peer -> data = (char *)"Client information";
	  break;
	case ENET_EVENT_TYPE_RECEIVE:
	  {
	    printf ("A packet of length %zu containing %s was received from %s on channel %u.\n",
		    event.packet -> dataLength,
		    event.packet -> data,
		    event.peer -> data,
		    event.channelID);
	    if (request.ParseFromArray(event.packet->data, event.packet->dataLength))
	      {
		std::cout << "Parse Good" << std::endl;
		switch (request.content())
		  {
		  case MasterServerRequest::GETSERVERS:
		    sendServers(event.peer);
		    break;
		  case MasterServerRequest::GETIP:
		    sendServer(event.peer, request.id());
		    break;
		  case MasterServerRequest::CREATESERVER:
		    createServer(event.peer, request.port(), request.info());
		    break;
		  case MasterServerRequest::DELETESERVER:
		    deleteServer(event.peer, request.port());
		    break;
		  default:
		    break;
		  }
	      }
	    /* Clean up the packet now that we're done using it. */
	    enet_packet_destroy (event.packet);
	    break;
	  }
	case ENET_EVENT_TYPE_DISCONNECT:
	  printf ("%s disconnected.\n", event.peer -> data);
	  /* Reset the peer's client information. */
	  event.peer -> data = NULL;
	default:
	  break;
        }
    }
}
