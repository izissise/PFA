#include <iostream>
#include <stdexcept>

#include "Unused.hpp"

void printv(const char *s)
{
  while (*s)
    {
      if (*s == '%')
        {
          if (*(s + 1) == '%')
            ++s;
          else
            throw std::runtime_error("invalid format string: missing arguments");
        }
      std::cout << *s++;
    }
}

template<typename T, typename... Args>
void printv(const char *s, T value, Args... args)
{
  while (*s)
    {
      if (*s == '%')
        {
          if (*(s + 1) == '%')
            ++s;
          else
            {
              std::cout << value;
              printv(s + 1, args...); // call even when *s == 0 to detect extra arguments
              return;
            }
        }
      std::cout << *s++;
    }
  throw std::logic_error("extra arguments provided to printf");
}

int main(UNUSED int ac, UNUSED char *av[])
{
  printv("Hello % the number % is % and it using variadic template !\n", "world", 5, 8);
  return 0;
}
