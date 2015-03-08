#include <iostream>
#include "Network.hpp"
#include "Unused.hpp"
#include "masterServer.hpp"

int main(UNUSED int ac, UNUSED char **av)
{
  try {
    MasterServer main;

    main.run();
    return (0);
  } catch (NetworkException &e) {
    std::cerr << e.what() << std::endl;
    return (1);
  }
}
