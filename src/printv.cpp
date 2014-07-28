#include "printv.hpp"

void printv(std::ostream& f, const std::string& s)
{
  size_t prevPos;;
  size_t pos;

  pos = 0;
  prevPos = 0;
  while ((pos = s.find('%', prevPos)) != std::string::npos)
    {
      f << s.substr(prevPos, pos - prevPos);
      if (s[pos + 1] != '%')
        throw std::runtime_error("invalid format string: missing arguments");
      else
        {
          ++pos;
          f << '%';
        }
      prevPos = pos + 1;
    }
  f << s.substr(prevPos);
}
