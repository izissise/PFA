#include "ClientProtocol.hpp"

ClientProtocol::ClientProtocol()
{
  _func[ProtocolMessage::SETTING] = &ClientProtocol::handleSetting;
}

ClientProtocol::~ClientProtocol()
{
  
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
  for (int i = 0;i < set.settingentry_size();++i)
    std::cout << "[" << set.settingentry(i).key() << "] = [" << set.settingentry(i).value() << "]" << std::endl;
}