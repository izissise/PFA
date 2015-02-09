#include "masterServer.hpp"

MasterServer::MasterServer()
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
      fprintf (stderr,
               "An error occurred while trying to create an ENet server host.\n");
      exit (EXIT_FAILURE);
  }
  std::cout << "Server start on " << address.port << std::endl;
}

MasterServer::~MasterServer()
{
  if (_server)
    enet_host_destroy(_server);
  enet_deinitialize();
}

void MasterServer::run()
{
  ENetEvent event;

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
        printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
                event.packet -> dataLength,
                event.packet -> data,
                event.peer -> data,
                event.channelID);
        /* Clean up the packet now that we're done using it. */
        enet_packet_destroy (event.packet);

        break;

    case ENET_EVENT_TYPE_DISCONNECT:
        printf ("%s disconnected.\n", event.peer -> data);
        /* Reset the peer's client information. */
        event.peer -> data = NULL;
    }
  }
}
