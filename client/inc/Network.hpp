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
	void	sendPacket(const std::string &packet, int channel);
	int		pollEvent(ENetEvent *event, int timeout);

private:
	ENetHost	*_host;
	ENetPeer	*_peer;
};

#endif