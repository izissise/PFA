#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "ServerSettings.hpp"
#include "Chunk.hpp"
#include "Vector2.hpp"

class World
{
public:
  World(ServerSettings &cvars);
  ~World() = default;

private:
  ServerSettings	&_cvars;
  std::vector<Chunk *>	_loadedChunks;
};

#endif /* WORLD_H */
