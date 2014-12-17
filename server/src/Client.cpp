#include "Client.hpp"

Client::Client(ENetPeer *peer)
  : _peer(peer),
    _actions(static_cast<int>(Action::Last), false),
    _initialized(false)
{
  std::cout << peer << std::endl;
}

Client::~Client()
{

}

void Client::sendPacket(enet_uint8 chan, const std::string &packet) const
{
  ENetPacket *message = enet_packet_create(packet.c_str(), packet.size(), ENET_PACKET_FLAG_RELIABLE);
  if (message == NULL || enet_peer_send(_peer, chan, message) != 0)
    throw (NetworkException("Cannot send the message"));
}

ENetPeer *Client::getPeer()
{
  return (_peer);
}

ClientEntity	&Client::getEntity()
{
  return _entity;
}

ClientInfo	&Client::getInfo()
{
  return _info;
}

const std::vector<bool>	&Client::getActions() const
{
  return _actions;
}

bool	Client::isInitialized() const
{
  return _initialized;
}

void	Client::setAction(unsigned int actionIdx, bool state)
{
  if (actionIdx >= _actions.size())
    throw std::out_of_range("Action index is out of range");
  _actions[actionIdx] = state;
}

void	Client::initialize()
{
  _initialized = true;
}
