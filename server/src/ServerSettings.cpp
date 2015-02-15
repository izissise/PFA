#include "ServerSettings.hpp"

ServerSettings::ServerSettings()
: ACvar()
{
  ACvar::addCvar("com_gameFps", new t_cvar({"20", "300"}, "60", "125", Number));
  ACvar::addCvar("s_seed", new t_cvar({"0", "4294967295"}, "0", "25341", Number));
  ACvar::addCvar("sv_ip", new t_cvar({}, "ip", "ServerIp", String, Restriction::None));
  ACvar::addCvar("sv_port", new t_cvar({"0", "65535"}, "27960", "27960", Number));
  ACvar::addCvar("sv_hostname", new t_cvar({}, "ServerName", "ServerName", String, Restriction::None));
  ACvar::addCvar("sv_slot", new t_cvar({"1", "4294967295"}, "30", "30", Number));
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
    cvar->set_allocated_default_(new std::string(it.second->defaultValue));
    cvar->set_allocated_min(new std::string(it.second->restrictValue[0]));
    cvar->set_allocated_max(new std::string(it.second->restrictValue[1]));
    cvar->set_allocated_value(new std::string(it.second->value));
    cvar->set_type(it.second->type);

    entry->set_allocated_cvar(cvar);
  }
  msg.set_content(ProtocolMessage::SETTING);
  msg.set_allocated_settings(set);
  msg.SerializeToString(&ret);
  return (ret);
}
