#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "ServerSettings.hpp"
#include "Client.hpp"
#include "Chunk.hpp"
#include "Vector2.hpp"

class World : public ISerialize
{
public:
  World(ServerSettings &cvars);
  ~World() = default;

  Chunk		*loadChunk(const std::vector<Client *> &clients, int x, int y);
  inline Chunk	*loadChunk(const std::vector<Client *> &clients,
			   const Vector2i &pos) { return loadChunk(clients, pos.x, pos.y); }

  Chunk		*getChunk(int x, int y) const;
  inline Chunk	*getChunk(const Vector2i &pos) const { return getChunk(pos.x, pos.y); }
  bool		isChunkLoaded(const Vector2i &pos) const;
  bool		isChunkLoaded(int x, int y) const { return isChunkLoaded({x, y}); }


  std::string	serialize() { return std::string(); };
  std::string	serialize(const std::vector<Vector2i> &chunkIds);
  std::string	serialize(const std::vector<Vector2i> &chunkIds,
			  const std::vector<Vector2i> &positions,
			  const std::vector<Vector2i> &sizes);

public:
  static const float	chunkDist;

private:
  void	releaseChunk(int x, int y);
  void	releaseChunk(const Chunk *const chunk);
  float	getClosestPlayer(const std::vector<Client *> &clients,
			 int x, int y) const;
  unsigned int	removeUnusedChunks(const std::vector<Client *> &clients);

private:
  ServerSettings	&_cvars;
  std::vector<Chunk *>	_loadedChunks;
};

#endif /* WORLD_H */
