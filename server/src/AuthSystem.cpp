#include <fstream>
#include "AuthSystem.hpp"
#include "StringUtils.hpp"
#include "GuidGenerator.hpp"
#include "Spawner.hpp"
#include "ServerResponse.pb.h"

AuthSystem::AuthSystem(World &world, std::vector<Client *> &clients) :
  _clients(clients), _world(world)
{
  _func[ClientMessage::CONNECTION] = &AuthSystem::handleConnection;
  _func[ClientMessage::PING] = &AuthSystem::ping;
  _func[ClientMessage::GETPLAYER] = &AuthSystem::getPlayer;
}

void	AuthSystem::parseCmd(const void *data, int size,
			     ENetPeer *peer)
{
  ClientMessage          packet;

  if (packet.ParseFromString(std::string((char *)data, size)))
    {
      ClientMessage::PacketContent  act = packet.content();
      auto it = _func.find(act);

      if (it != _func.end())
	{
	  (this->*(it->second))(packet, peer);
	}
    }
  else
    std::cerr << "Cannot DeSerialize Data" << std::endl;

}

void	AuthSystem::handleConnection(const ClientMessage &message,
				     ENetPeer *peer)
{
  const ConnectionMessage	&coInfo = message.co();
  const std::string		&userId = coInfo.userid();
  std::string			newId;
  Client			*client;

  std::cout << "USERID " << userId << std::endl;
  if (userId == "SERVER")
    return ;
  else
    {
      client = new Client(peer);
      peer->data = client;
      notify(t_event(client));
    }
  if (userId.empty())
    client->getInfo().setId(userId);
  else
    {
      generateNewId(newId);
      client->getInfo().setId(newId);
    }
  auto it = std::find_if(_clients.begin(), _clients.end(), [client, &userId](Client *cl)
			 {
			   return (client != cl && cl->getInfo().getId() == userId);
			 });
  if (it != _clients.end())
    {
      enet_peer_disconnect(client->getPeer(), 0);
      return ;
    }
  loadClientProfile(client, userId);
  client->initialize();
  sendClientProfile(client, newId);
}

void	AuthSystem::ping(const ClientMessage &message,
			 ENetPeer *peer)
{
  std::string	packet;
  Client	cl(peer);

  message.SerializeToString(&packet);
  cl.sendPacket(0, packet);
}

void	AuthSystem::loadClientProfile(Client *client, const std::string &userId)
{
  std::fstream			file;
  StringUtils			utils;
  std::vector<std::string>	clientInfo;
  std::vector<std::string>	vec;
  ClientEntity			&clEnt = client->getEntity();

  if (userId.empty())
    {
      spawnClient(_clients, client);
      return ;
    }
  file.open(LOGFILE, std::ios::binary | std::ios::in);
  if (!file) // Can be the first client joining
    {
      std::cout << "File doesnt exist yet" << std::endl;
      spawnClient(_clients, client);
      return ;
    }
  for (std::string line; getline(file, line);)
    {
      if (line.find(userId) != std::string::npos)
	{
	  std::cout << "Found in database -> load client's data" << std::endl;
	  utils.split(line, ';', clientInfo);
	  utils.split(clientInfo.at(1), ' ', vec);
	  clEnt.setChunkId({std::stoi(vec.at(0)), std::stoi(vec.at(1))});
	  vec.clear();
	  utils.split(clientInfo.at(2), ' ', vec);
	  clEnt.setPosition({std::stof(vec.at(0)), std::stof(vec.at(1))});
	  file.close();
	  return ;
	}
    }
  file.close();
  spawnClient(_clients, client);
}

void	AuthSystem::spawnClient(const std::vector<Client *> &clients,
				Client *client)
{
  Spawner	spawner(_world);

  spawner.spawnClient(clients, client);
}

void	AuthSystem::generateNewId(std::string &guid)
{
  GuidGenerator	gen;

  guid = gen.generate();
}

void	AuthSystem::sendClientProfile(Client *client,
				      const std::string &newId)
{
  ProtocolMessage	msg;
  InitClient		*initInfo = new InitClient;
  Position		*pos = new Position;
  VectorInt		*chunkId = new VectorInt;
  VectorFloat		*clPos = new VectorFloat;
  std::string		*guid = new std::string(newId);
  ClientEntity		&clEnt = client->getEntity();
  std::string		serialized;

  chunkId->set_x(clEnt.getChunkId().x);
  chunkId->set_y(clEnt.getChunkId().y);
  pos->set_allocated_chunkid(chunkId);

  clPos->set_x(clEnt.getPosition().x);
  clPos->set_y(clEnt.getPosition().y);
  pos->set_allocated_pos(clPos);

  initInfo->set_allocated_pos(pos);
  initInfo->set_allocated_guid(guid);
  msg.set_content(ProtocolMessage::CLINIT);
  msg.set_allocated_clinit(initInfo);
  msg.SerializeToString(&serialized);
  client->sendPacket(0, serialized);
}

void    AuthSystem::getPlayer(const ClientMessage &message UNUSED,
                              ENetPeer *peer)
{
  Client cl(peer);
  ServerResponse response;
  std::string str;

  std::cout << "Receive Packet" << std::endl;
  response.set_content(ServerResponse::PLAYER);
  response.set_player(_clients.size());
  response.SerializeToString(&str);
  cl.sendPacket(0, str);
}
