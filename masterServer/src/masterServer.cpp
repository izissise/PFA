#include <thread>
#include "masterServer.hpp"
#include "IpToCountrySlow.hpp"

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
                 "(ip TEXT, port TEXT, name TEXT, currentPlayer INTEGER, slots INTEGER, country TEXT, lastUpdate INTEGER, "
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
        std::cout << "exception: " << e.what() << std::endl;
    }
}

void MasterServer::getServerInfo(SQLite::Statement &query, ServerInfo *server)
{
    int laps = 2 * 60; // 5 minutes
    
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
        currentPlayer = getServerPlayer(ip, std::stoi(port));
        if (currentPlayer == (unsigned int)-1)
        {
            std::cerr << "The Server Cannot be reach" << std::endl;
        }

        SQLite::Statement st(_db, "UPDATE server SET currentPlayer = ?, lastUpdate = ? WHERE ip = ? AND port = ?");

        st.bind(1, (int)currentPlayer);
        st.bind(2, (int)std::time(NULL));
        st.bind(3, ip);
        st.bind(4, port);
        st.exec();
        
        std::cout << "UPDATE the database for the player" << std::endl;
    }
    
    server->set_ip(ip);
    server->set_port(port);
    server->set_name(name);
    server->set_currentplayer(currentPlayer);
    server->set_maxplayer(slots);
    server->set_country(country);

}

int MasterServer::getServerPlayer(const char *ip, int port) const
{
    ENetPeer *peer;
    ENetAddress address;
    ENetHost * client;
    int currentPlayer;
    
    client = enet_host_create (NULL /* create a client host */,
                               1 /* only allow 1 outgoing connection */,
                               2 /* allow up 2 channels to be used, 0 and 1 */,
                               57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
                               14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);
    
    enet_address_set_host (&address, ip);
    address.port = port;
    peer = enet_host_connect (client, & address, 2, 0);
    if (peer == NULL)
    {
        fprintf (stderr,
                 "No available peers for initiating an ENet connection.\n");
        exit (EXIT_FAILURE);
    }
    
    ENetEvent event;
    int i = 0;
    while (enet_host_service (client, & event, 1000) >= 0 && i < 3)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
            {
                ClientMessage request;
                std::string message;
                
                request.set_content(ClientMessage::GETPLAYER);
                request.SerializeToString(&message);
                ENetPacket * packet = enet_packet_create (message.data(),
                                                          message.size(),
                                                          ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(peer, 0, packet);
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE:
            {
                ServerResponse response;
                
                for (unsigned int i = 0; i < event.packet->dataLength; ++i)
                {
                    std::cout << (int)event.packet->data[i] << " ";
                }
                std::cout << std::endl;
                
                if (response.ParseFromArray(event.packet->data, event.packet->dataLength)
                    && response.content() == ServerResponse::PLAYER)
                {
                    std::cout << "Packet Player = " << response.player() << std::endl;
                    currentPlayer = response.player();
                    enet_peer_disconnect (peer, 0);
                    enet_host_destroy(client);
                    return currentPlayer;
                }
                else
                {
                    currentPlayer = -1;
                }
                break;
            }
            default:
                break;
        }
        ++i;
    }
    if (i == 3)
    {
        std::cout << "No Response Received" << std::endl;
        currentPlayer = -1;
    }
    enet_host_destroy(client);
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
        std::cout << "Delete" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

void MasterServer::getServer(ENetPeer *peer)
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
            
            std::string message;
            response.SerializeToString(&message);
            
            ENetPacket *packet = enet_packet_create(message.c_str(), message.size(),
                                                    ENET_PACKET_FLAG_RELIABLE);
            if (packet == nullptr || enet_peer_send(peer, 0, packet) != 0)
            {
                std::cerr << "Cannot be send"<< std::endl;
            }
        }
    }
    catch (std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
}


void MasterServer::parsePacket(ENetPacket *packet, ENetPeer *peer)
{
    MasterServerRequest request;
    if (request.ParseFromArray(packet->data, packet->dataLength))
    {
        std::cout << "Parse Good" << std::endl;
        switch (request.content()) {
            case MasterServerRequest::GETIP:
                getServer(peer);
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
                printf ("A new client connected from %s:%u.\n",
                        name,
                        event.peer -> address.port);
                /* Store any relevant client information here. */
                event.peer -> data = strdup(name);
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE:
            {
                std::thread t(&MasterServer::parsePacket, this, event.packet, event.peer);
                t.detach();
                
                printf ("A packet of length %zu containing %s was received from %s on channel %u.\n",
                        event.packet -> dataLength,
                        event.packet -> data,
                        event.peer -> data,
                        event.channelID);
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
