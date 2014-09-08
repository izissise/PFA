/**
 * \file main.c
 * \brief Main binary.
 * \date 12 august 2014
 *
 * It is the source of the main binary of client or server (one will be needed for both),
 * The infinite loop of the game should be executed here and not in the lib (for testing purpose),
 * Also it would be good that a try catch block do some advanced error reporting.
 *
 */

#include <iostream>

#include <enet/enet.h>

#include "Config.h"
#include "Menu.hpp"
#include "Settings.hpp"
#include "Console.hpp"
#include "demangle.hpp"
#include "Unused.hpp"
#include "printv.hpp"

int	main(UNUSED int ac, UNUSED char *av[])
{

    Settings	set;
    Console     con(&set);
    Menu       	menu(set);

    printv(std::cout, "Program version: % %\n", xstr(PROJECT_VERSION), xstr(CURRENT_DATE));
    menu.run(set, con);

    /*ENetHost * client;
    client = enet_host_create (NULL,
                               1 ,
                               2 ,
                               57600 / 8 ,
                               14400 / 8 );
    if (client == NULL)
    {
        fprintf (stderr,
                 "An error occurred while trying to create an ENet client host.\n");
        exit (EXIT_FAILURE);
    }

    ENetAddress address;
    ENetEvent event;
    ENetPeer *peer;
    enet_address_set_host (&address, "127.0.0.1");
    address.port = 6060;
    peer = enet_host_connect (client, & address, 2, 0);
    if (peer == NULL)
    {
        fprintf (stderr,
                 "No available peers for initiating an ENet connection.\n");
        exit (EXIT_FAILURE);
    }
    while (enet_host_service (client, &event, 5000) >= 0)
    {
        if (event.type == ENET_EVENT_TYPE_CONNECT)
        {
            puts ("Connection to some.server.net:1234 succeeded.");
        }
    }
    enet_host_destroy(client); */

    return 0;
}
