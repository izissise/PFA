#include "Network.hpp"

Network::Network()
: _host(NULL), _peer(NULL)
{
	if (enet_initialize () != 0)
    throw NetworkException("An error occurred while initializing ENet.");
}

Network::~Network()
{
	disconnect();
	enet_deinitialize();
}

void Network::connect(const std::string &address, const std::string &port, int nbChannel)
{
	ENetAddress addr;
	std::stringstream ss(port);

	if (_host || _peer)
		disconnect();
	
	if ((_host = enet_host_create(NULL, 1, nbChannel, 57600 / 8, 14400 / 8)) == NULL)
		throw NetworkException("An error occurred while trying to create an ENet client host.");
	enet_address_set_host(&addr, address.c_str());
	ss >> addr.port;
	_peer = enet_host_connect(_host, &addr, 2, 0);
	if (_peer == NULL)
		throw NetworkException("No available peers for initiating an ENet connection.");
}

void Network::disconnect()
{
	if (_peer)
		enet_peer_reset(_peer);
	if (_host)
		enet_host_destroy(_host);
}

void Network::sendPacket(const std::string &packet, int channel)
{
	if (_host)
	{
		ENetPacket *p = enet_packet_create (packet.c_str(), packet.size() + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(_peer, channel, p);
	}
}

int Network::pollEvent(ENetEvent *event, int timeout)
{
	if (_host && _peer)
		return (enet_host_service(_host, event, timeout));
	return (-1);
}