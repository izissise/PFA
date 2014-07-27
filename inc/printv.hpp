#ifndef PRINTV_HPP_INCLUDED
# define PRINTV_HPP_INCLUDED

# include <iostream>
# include <string>
# include <stdexcept>

void printv(std::ostream& f, const std::string& s);

template<typename T, typename... Args>
void printv(std::ostream& f, const std::string& s, T value, Args... args)
{
  size_t prevPos;;
  size_t pos;

  pos = 0;
  prevPos = 0;
  while ((pos = s.find('%', prevPos)) != std::string::npos)
    {
      f << s.substr(prevPos, pos);
      if (s[pos + 1] != '%')
        {
          f << value;
          printv(f, s.substr(pos + 1), args...); // call even when *s == 0 to detect extra arguments
          return;
        }
      else
        {
          ++pos;
          f << '%';
        }
      prevPos = pos + 1;
    }
  throw std::logic_error("extra arguments provided to printf");
}

#endif // PRINTV_HPP_INCLUDED
