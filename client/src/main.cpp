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

//#include <enet/enet.h>
#include <SFML/Window.hpp>

#include "ClientMain.hpp"
#include "demangle.hpp"
#include "Unused.hpp"
#include "printv.hpp"

int	main(UNUSED int ac, UNUSED char *av[])
{
  ClientMain game;

  printv(std::cout, "Program version: % %\n", xstr(PROJECT_VERSION), xstr(CURRENT_DATE));
  game.run();
  return 0;
}
