#include "ServerSettings.hpp"

ServerSettings::ServerSettings()
: ACvar()
{
  ACvar::addCvar("com_gameFps", new t_cvar({"20, 300"}, "125", Number));
  ACvar::addCvar("s_seed", new t_cvar({"0, 4294967295"}, "25341", Number));
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
  ProtocolMessage  msg;
  std::string ret;
  SettingMessage *set = new SettingMessage();

  for (auto it : _cvars)
  {
    SettingMessage::SettingEntry  *entry = set->add_settingentry();
    
    entry->set_allocated_key(new std::string(it.first));
    entry->set_allocated_value(new std::string(it.second->value));
  }

  msg.set_action(ProtocolMessage::SETTING);
  msg.set_allocated_settings(set);
  msg.SerializeToString(&ret);
  return (ret);
}
