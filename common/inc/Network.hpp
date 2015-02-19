#ifndef _NETWORK_HPP_
# define _NETWORK_HPP_

# include <enet/enet.h>
# include <sstream>
# include <string>

# include "NetworkException.hpp"

class Network
{
public:
  Network();
  ~Network();

  void	connect(const std::string &address, const std::string &port, int nbChannel);
  void	disconnect();
  void	sendPacket(enet_uint8 chan, const std::string &message) const;
  int	pollEvent(ENetEvent *event, int timeout);
  void	adjustNetworkSettings(enet_uint32 incomingBandwidth,
			      enet_uint32 outgoingBandwidth);
  ENetHost	*getHost();
  void		setConnected();
  bool		isConnected() const;
  bool		isOnline() const;

private:
  ENetHost	*_host;
  ENetPeer	*_peer;
  bool		_connected;
};

#endif
