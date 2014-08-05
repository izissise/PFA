#include <iostream>

#include "Config.h"
#include "Unused.hpp"
#include "printv.hpp"

int main(UNUSED int ac, UNUSED char *av[])
{
  printv(std::cout, "Program version: %\n", xstr(PROJECT_VERSION));
  return 0;
}
