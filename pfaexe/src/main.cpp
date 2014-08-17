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

#include "Config.h"
#include "Unused.hpp"
#include "printv.hpp"
#include "Menu.hpp"

int	main(UNUSED int ac, UNUSED char *av[])
{
  Menu	menu;

  printv(std::cout, "Program version: % %\n", xstr(PROJECT_VERSION), xstr(CURRENT_DATE));
  // testSfml();
  menu.run();
  return 0;
}
