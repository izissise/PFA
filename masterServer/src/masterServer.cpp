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
        _db.exec("CREATE TABLE IF NOT EXISTS server (ip TEXT, port TEXT)");
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

void MasterServer::createServer()
{
    try
    {
        SQLite::Transaction transaction(_db);
        
        int nb = _db.exec("INSERT INTO server VALUES (\"10.10.253.252\", \"8000\")");
        
        std::cout << "INSERT INTO server VALUES (\"10.10.253.252\", \"8000\"), returned " << nb << std::endl;
        
        // Commit transaction
        transaction.commit();
    }
    catch (std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

void MasterServer::deleteServer()
{
    std::cout << "Delete" << std::endl;
    
}

void MasterServer::getServer()
{
    try
    {
        SQLite::Statement   query(_db, "SELECT * FROM server");
        
        // Loop to execute the query step by step, to get rows of result
        while (query.executeStep())
        {
            // Demonstrate how to get some typed column value
            const char* ip     = query.getColumn(0);
            const char* port   = query.getColumn(1);
            
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
                std::string data = std::string((char *)event.packet->data);
                if (data == "getip")
                {
                    getServer();
                }
                else if (data == "createServer")
                {
                    createServer();
                }
                else if (data == "deleteServer")
                {
                    createServer();
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