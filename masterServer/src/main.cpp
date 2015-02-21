#include <iostream>
#include <Network.hpp>
#include "masterServer.hpp"

int main(int, char **)
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
