#include <iostream>

#if defined(__GNUC__)
# define UNUSED __attribute__((unused))
#elif defined(WIN32)
# define UNUSED __pragma(warning(suppress:4100))
#elif defined(WIN64)
# define UNUSED __pragma(warning(suppress:4100))
#elif defined(__APPLE__)
# define UNUSED __attribute__((unused))
#endif

int main(UNUSED int ac, UNUSED char *av[])
{
  std::cout << "Hello World !" << std::endl;
  return 0;
}
