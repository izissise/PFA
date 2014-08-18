#include "Cvar.hpp"
#include "Exception.hpp"

CvarList::CvarList()
{
  addCvar("r_width", new t_cvar({"600", "1", "1920"}, "1600", Number));
  addCvar("r_height", new t_cvar({"800", "1", "1080"}, "900", Number));
}

CvarList::~CvarList()
{
}

const std::string	&CvarList::getCvar(const std::string &name) const
{
  auto it = _cvars.find(name);

  if (it == _cvars.end())
    throw(Exception("Key not registered"));
  return it->second->value;
}

bool	CvarList::addCvar(const std::string &name, t_cvar *cvar)
{
  auto ret = _cvars.insert(std::pair<std::string, t_cvar *>(name, cvar));

  return ret.second;
}
