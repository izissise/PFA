#include <functional>

#include "StringUtils.hpp"

StringUtils::StringUtils()
{
}

StringUtils::~StringUtils()
{
}

void	StringUtils::split(const std::string &s, char delim,
			   std::vector<std::string> &elems) const
{
  std::stringstream	ss(s);
  std::string		item;

  while (std::getline(ss, item, delim))
    {
      if (!item.empty())
	elems.push_back(item);
    }
}

void	StringUtils::tokenString(const std::string &cmd, std::vector<std::string> &tokens) const
{
  std::size_t	last = 0;
  std::size_t	i;
  std::function<void ()> cutFunc = [&]()
    {
      if (cmd.at(last) == '"')
	++last;
      if (cmd.at(i - 1) == '"')
	tokens.push_back(cmd.substr(last, i - last - 1));
      else
	tokens.push_back(cmd.substr(last, i - last));
    };

  for (i = 0; i < cmd.length();)
    {
      char c = cmd[i];
      if (c == ' ')
	{
	  if (i != last)
	    cutFunc();
	  while (i < cmd.length() && cmd[i] == ' ')
	    ++i;
	  last = i;
	  continue ;
	}
      if (c == '\"')
	{
	  std::size_t tmp = cmd.find("\"", i + 1);

	  if (tmp != std::string::npos)
	    i = tmp; // pass the quote
	}
      ++i;
    }
  if (i != last)
    cutFunc();
}
