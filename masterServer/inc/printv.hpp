#ifndef PRINTV_HPP_INCLUDED
# define PRINTV_HPP_INCLUDED

# include <iostream>
# include <string>
# include <stdexcept>
# include <sstream>

/**
 * \fn std::ostream& printv(std::ostream& f, const std::string& s)
 * \brief Printf like function but using variadic template.
 * \param[out] f The output stream (ex: std::cout).
 * \param[in] s The format string each '%%' is an argument.
 * \return f so you can reuse it.
 *
 * For the format string, when you want to pass some arguments, you only have
 * to pass a '%%' without precising the type, you can also write a % with '%%%'.
 *
 */

std::ostream& printv(std::ostream& f, const std::string& s)
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
  return f;
}

template<typename T, typename... Args>
std::ostream& printv(std::ostream& f, const std::string& s, T value, Args... args)
{
  size_t prevPos;
  size_t pos;

  pos = 0;
  prevPos = 0;
  while ((pos = s.find('%', prevPos)) != std::string::npos)
    {
      f << s.substr(prevPos, pos - prevPos);
      if (s[pos + 1] != '%')
        {
          f << value;
          printv(f, s.substr(pos + 1), args...);
          return f;
        }
      else
        {
          ++pos;
          f << '%';
        }
      prevPos = pos + 1;
    }
  throw std::logic_error("extra arguments provided to printv");
  return f;
}

#endif // PRINTV_HPP_INCLUDED
