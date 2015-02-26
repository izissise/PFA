#include "ServerSettings.hpp"

ServerSettings::ServerSettings()
  : ACvar()
{
  ACvar::addCvar("com_gameFps", new t_cvar({"20", "300"}, "60", "125", Number));
  ACvar::addCvar("s_seed", new t_cvar({"0", "4294967295"}, "0", "25341", Number));
  ACvar::addCvar("sv_port", new t_cvar({"0", "65535"}, "6000", "6000", Number));
  ACvar::addCvar("sv_hostname", new t_cvar({}, "ServerName", "ServerName", String, Restriction::None));
  ACvar::addCvar("sv_slot", new t_cvar({"1", "4294967295"}, "30", "30", Number));
  ACvar::addCvar("sv_masterIP", new t_cvar({"127.0.0.1"/*"62.210.136.56"*/}, "127.0.0.1",
                                           "127.0.0.1", String, Restriction::Value));
  ACvar::addCvar("sv_masterPort", new t_cvar({"25255"}, "25255",
                                             "25255", String, Restriction::Value));
  ACvar::addCvar("sv_debug", new t_cvar({"0", "1"}, "0", "0", Number));
  ACvar::addCvar("sv_quiet", new t_cvar({"0", "1"}, "0", "0", Number));
}

ServerSettings::~ServerSettings()
{
}

void    ServerSettings::setCvar(const std::string &name, const std::string &value)
{
  ACvar::setCvar(name, value);
}

std::string     ServerSettings::serialize()
{
  ProtocolMessage       msg;
  std::string           ret;
  SettingMessage        *set = new SettingMessage();

  for (auto it : _cvars)
    {
      cvarData  *cvar = set->add_cvars();

      cvar->set_allocated_key(new std::string(it.first));
      for (auto &rValues : it.second->restrictValue)
        cvar->add_restrictedvalues(rValues);
      cvar->set_allocated_default_(new std::string(it.second->defaultValue));
      cvar->set_allocated_value(new std::string(it.second->value));
      cvar->set_type(it.second->type);
      cvar->set_restrictiontype(static_cast<int>(it.second->restrictType));
    }
  msg.set_content(ProtocolMessage::SETTING);
  msg.set_allocated_settings(set);
  msg.SerializeToString(&ret);
  return (ret);
}
