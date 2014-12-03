#include "ServerSettings.hpp"

ServerSettings::ServerSettings()
: ACvar()
{
}

ServerSettings::~ServerSettings()
{
}

void	ServerSettings::setCvar(const std::string &name, const std::string &value)
{
  t_event event;

  ACvar::setCvar(name, value);
  event.change = name;
  notify(event);
}

std::string	ServerSettings::serialize()
{
  ProtocolMessage	msg;
  std::string		ret;
  SettingMessage	*set(new SettingMessage());

  set->set_gamefps( std::stoi(_cvars["com_gameFps"]->value) );
  msg.set_action(ProtocolMessage::SETTING);
  msg.set_allocated_settings(set);
  msg.SerializeToString(&ret);
  return (ret);
}
