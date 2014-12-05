#include "ServerSettings.hpp"

ServerSettings::ServerSettings()
: ACvar()
{
  ACvar::addCvar("com_gameFps", new t_cvar({"60", "20", "300"}, "125", Number));
  ACvar::addCvar("s_seed", new t_cvar({"0", "0", "4294967295"}, "25341", Number));
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
  SettingMessage	*set = new SettingMessage();

  for (auto it : _cvars)
  {
    SettingMessage::SettingEntry  *entry = set->add_settingentry();
    SettingMessage::SettingEntry::Cvar *cvar = new SettingMessage::SettingEntry::Cvar();

    entry->set_allocated_key(new std::string(it.first));
    cvar->set_allocated_default_(new std::string(it.second->restrictValue[0]));
    cvar->set_allocated_min(new std::string(it.second->restrictValue[1]));
    cvar->set_allocated_max(new std::string(it.second->restrictValue[2]));
    cvar->set_allocated_value(new std::string(it.second->value));
    cvar->set_type(it.second->type);

    entry->set_allocated_cvar(cvar);
  }
  msg.set_action(ProtocolMessage::SETTING);
  msg.set_allocated_settings(set);
  msg.SerializeToString(&ret);
  return (ret);
}
