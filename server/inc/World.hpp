#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "ServerSettings.hpp"
#include "Client.hpp"
#include "Chunk.hpp"
#include "Vector2.hpp"

class World
{
public:
  World(ServerSettings &cvars);
  ~World() = default;

  void		loadChunk(int x, int y);
  Chunk		*getChunk(int x, int y) const;

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
