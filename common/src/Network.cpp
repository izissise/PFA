#include "Network.hpp"

Network::Network()
  : _host(nullptr), _peer(nullptr), _connected(false)
{
  if (enet_initialize() < 0)
    throw NetworkException("An error occurred while initializing ENet.");
}

Network::~Network()
{
  disconnect();
  enet_deinitialize();
}

void	Network::connect(const std::string &address, const std::string &port, int nbChannel)
{
  ENetAddress addr;
  std::stringstream ss(port);

  if (isOnline())
    disconnect();
  if ((_host = enet_host_create(NULL, 1, nbChannel, 0, 0)) == NULL)
    throw NetworkException("An error occurred while trying to create an ENet client host.");
  enet_address_set_host(&addr, address.c_str());
  ss >> addr.port;
  _peer = enet_host_connect(_host, &addr, nbChannel, 0);
  if (_peer == nullptr)
    throw NetworkException("No available peers for initiating an ENet connection.");
}

void	Network::disconnect()
{
  if (_peer)
    enet_peer_reset(_peer);
  if (_host)
    enet_host_destroy(_host);
  _peer = nullptr;
  _host = nullptr;
  _connected = false;
}

void	Network::sendPacket(enet_uint8 chan, const std::string &message) const
{
  if (_host)
    {
      ENetPacket *packet = enet_packet_create(message.c_str(), message.size(),
					      ENET_PACKET_FLAG_RELIABLE);
      if (packet == nullptr || enet_peer_send(_peer, chan, packet) < 0)
	{
	  throw (NetworkException("Cannot send the message"));
	}
    }
}

int	Network::pollEvent(ENetEvent *event, int timeout)
{
  if (_host && _peer)
    return (enet_host_service(_host, event, timeout));
  return (-1);
}

void	Network::adjustNetworkSettings(enet_uint32 incomingBandwidth,
				       enet_uint32 outgoingBandwidth)
{
  if (_host)
    enet_host_bandwidth_limit(_host, incomingBandwidth, outgoingBandwidth);
  else
    throw (NetworkException("Must call connect methode before adjusting network settings"));
}

ENetHost	*Network::getHost()
{
  return _host;
}

void	Network::setConnected()
{
  _connected = true;
}

bool	Network::isConnected() const
{
  return _connected;
}

bool	Network::isOnline() const
{
  return _host && _peer;
}
