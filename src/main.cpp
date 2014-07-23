#include <iostream>

#define UNUSED __attribute__((unused))

int main(UNUSED int ac, UNUSED char *av[])
{
  std::cout << "Hello World !" << std::endl;
  return 0;
}
