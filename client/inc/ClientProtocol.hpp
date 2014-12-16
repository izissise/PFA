#ifndef _CLIENTPROTOCOL_HPP_
# define _CLIENTPROTOCOL_HPP_

# include <iostream>

# include "World.hpp"
# include "Settings.hpp"
# include "Network.hpp"
# include "NetworkException.hpp"
# include "ProtocolMessage.pb.h"
# include "ClientMessage.pb.h"
# include "ThreadPool.hpp"

# define GUIDFILE "../guid.txt"

using namespace google::protobuf;

class ClientProtocol
{
public:
  ClientProtocol(Network &net, ThreadPool &tPool);
  ~ClientProtocol();

  void  parseCmd(const void *data, int size);
  void  setSetting(Settings *set);
  void  setWorld(const std::shared_ptr<World> &world);

private:
  typedef void	(ClientProtocol::*func)(const ProtocolMessage &packet);
  void  handleSetting(const ProtocolMessage &packet);
  void	initClient(const ProtocolMessage &packet);
  void	fillChunk(const ProtocolMessage &packet);
  void	handleDisplacements(const ProtocolMessage &packet);
  void	queryInitialChunks();
  void	queryChunks(const std::vector<Vector2i> &chunkIds) const;
  void	getNewChunks();
  void	writeNewGuid(const std::string &guid) const;

  std::map<ProtocolMessage::PacketContent, func>	_func;
  std::shared_ptr<World>	_world;
  Network			&_socket;
  Settings			*_set;
  ThreadPool			&_threadPool;
};

#endif
