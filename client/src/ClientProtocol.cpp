#include "ClientProtocol.hpp"

ClientProtocol::ClientProtocol()
{
  _func[ProtocolMessage::SETTING] = &ClientProtocol::handleSetting;
}

ClientProtocol::~ClientProtocol()
{
  
}

void ClientProtocol::setSetting(Settings *set)
{
  _set = set;
}

void ClientProtocol::setWorld(const std::shared_ptr<World> &world)
{
  _world = world;
}

void  ClientProtocol::parseCmd(const void *data, int size)
{
  ProtocolMessage          packet;
  
  if (packet.ParseFromString(std::string((char *)data, size)))
  {
    ProtocolMessage::Action  act = packet.action();
    
    if (_func.find(act) != _func.end())
      (this->*_func[act])(packet);
  }
  else
    std::cerr << "Cannot DeSerialize Data" << std::endl;
}

void  ClientProtocol::handleSetting(ProtocolMessage packet)
{
  if (!packet.has_settings())
    return ;
  std::cout << "Handle Setting" << std::endl;

  SettingMessage set = packet.settings();
  for (int i = 0; i < set.settingentry_size(); ++i)
  {
    SettingMessage::SettingEntry::Cvar cvar = set.settingentry(i).cvar();
    t_cvar *entry = new t_cvar( {{std::string(cvar.default_()), std::string(cvar.min()), std::string(cvar.max())}},
                               cvar.value(), static_cast<cvarType>(cvar.type()));

    if (!_set->getCvarList().addCvar(set.settingentry(i).key(), entry))
    {
      _set->getCvarList().setCvar(set.settingentry(i).key(), cvar.value());
      delete entry;
    }
  }
}