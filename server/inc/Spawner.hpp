#ifndef _SPAWNER_H_
#define _SPAWNER_H_

#include "World.hpp"

class Spawner
{
public:
  Spawner(World &world);
  virtual ~Spawner();

  void	spawnClient(const std::vector<Client *> &clients,
		    Client *Client);

private:
  void	moveToSurface(Vector2i &chunkId,
		      const std::vector<Client *> &clients) const;
  int	isChunkSpawnable(Chunk *chunk,
			 const std::vector<Client *> &clients);
  bool	placePlayerOnSurface(Chunk *chunk,
			     unsigned int chunkPart,
			     Vector2u &chunkPos);
private:
  World		&_world;
};

#endif /* _SPAWNER_H_ */
