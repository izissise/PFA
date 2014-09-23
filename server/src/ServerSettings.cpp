#include "ServerSettings.hpp"

ServerSettings::ServerSettings()
: CvarList()
{
  
}

ServerSettings::~ServerSettings()
{
  
}

void ServerSettings::setCvar(const std::string &name, const std::string &value)
{
  t_event event;

  CvarList::setCvar(name, value);
  event.change = name;
  notify(event);
}