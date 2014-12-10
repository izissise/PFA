#include "StringUtils.hpp"

StringUtils::StringUtils()
{
}

StringUtils::~StringUtils()
{
}

void	StringUtils::split(const std::string &s, char delim,
			   std::vector<std::string> &elems)
{
  std::stringstream	ss(s);
  std::string		item;

  while (std::getline(ss, item, delim))
    {
      if (!item.empty())
	elems.push_back(item);
    }
}
