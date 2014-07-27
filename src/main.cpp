#include <iostream>

#include "Unused.hpp"
#include "printv.hpp"

int main(UNUSED int ac, UNUSED char *av[])
{
  printv("Hello % the number % is % and it using variadic template !\n", "world", 5, 8);
  return 0;
}
