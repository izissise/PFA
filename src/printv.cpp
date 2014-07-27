#include "printv.hpp"

void printv(const std::string& s)
{
  size_t prevPos;;
  size_t pos;

  pos = 0;
  prevPos = 0;
  while ((pos = s.find('%', prevPos)) != std::string::npos)
    {
      std::cout << s.substr(prevPos, pos);
      if (s[pos + 1] != '%')
        throw std::runtime_error("invalid format string: missing arguments");
      else
        {
          ++pos;
          std::cout << '%';
        }
      prevPos = pos + 1;
    }
  std::cout << s.substr(prevPos);
}
