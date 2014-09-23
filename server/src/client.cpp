#include "client.hpp"

Client::Client(ENetPeer *peer)
: _peer(peer)
{
    std::cout << peer << std::endl;
    std::cout << _peer << std::endl;
}

Client::~Client()
{

}

void Client::sendPacket(enet_uint8 chan, const std::string &packet) const
{
    ENetPacket *message = enet_packet_create(packet.c_str(), packet.size() + 1, ENET_PACKET_FLAG_RELIABLE);
    if (message == NULL || enet_peer_send(_peer, chan, message) != 0)
        throw (NetworkException("Cannot send the message"));
}

const ENetPeer *Client::getPeer() const
{
    return (_peer);
}
