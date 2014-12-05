#include <algorithm>
#include "World.hpp"
#include "Perlin.h"
#include "FastMath.h"

const float World::chunkDist = 2.f;

World::World(ServerSettings &cvars) :
  _cvars(cvars)
{
  noise::setSeed(std::stoi(cvars.getCvar("s_seed")));
  noise::initPerm();
}

Chunk	*World::loadChunk(const std::vector<Client *> &clients, int x, int y)
{
  Chunk	*chunk = new Chunk;

  chunk->load(x, y);
  _loadedChunks.push_back(chunk);
  removeUnusedChunks(clients);
  return chunk;
}

Chunk	*World::getChunk(int x, int y) const
{
  Vector2i	pos(x, y);
  auto it = std::find_if(_loadedChunks.begin(), _loadedChunks.end(),
			 [pos](Chunk *chunk)
			 {
			   return (chunk->getPosition() == pos);
			 });
  return ((it == _loadedChunks.end()) ? nullptr : *it);
}

void	World::releaseChunk(int x, int y)
{
  Vector2i	pos(x, y);
  auto it = std::find_if(_loadedChunks.begin(), _loadedChunks.end(),
			 [pos](Chunk *chunk)
			 {
			   return (chunk->getPosition() == pos);
			 });

  if (it == _loadedChunks.end())
    return ;
  _loadedChunks.erase(it);
}

void	World::releaseChunk(const Chunk *const chunk)
{
  auto it = std::find(_loadedChunks.begin(), _loadedChunks.end(), chunk);

  if (it != _loadedChunks.end())
    _loadedChunks.erase(it);
}

float	World::getClosestPlayer(const std::vector<Client *> &clients,
				int x, int y) const
{
  Vector2f	pos;
  Vector2i	chunkId;
  float		distance;
  float		closest = -1.f;
  ClientEntity	clEnt;

  // Going up makes the Y indice increment
  x += Chunk::width / 2;
  y -= Chunk::height / 2;
  for (const auto cl : clients)
    {
      clEnt = cl->getEntity();
      pos = clEnt.getPosition();
      chunkId = clEnt.getChunkId();
      chunkId.x *= Chunk::width;
      chunkId.y *= Chunk::height;
      pos = Vector2f(chunkId.x + pos.x, chunkId.y - pos.y);
      distance = pointDist(std::abs(pos.x - x), std::abs(pos.y - y));
      if (closest == -1 || distance < closest)
	closest = distance;
    }
  return closest;
}

unsigned int	World::removeUnusedChunks(const std::vector<Client *> &clients)
{
  unsigned int	counter = 0;
  float		maxDist;

  maxDist = pointDist(Chunk::width * TileCodex::tileSize,
		      Chunk::height * TileCodex::tileSize)
    * World::chunkDist;
  _loadedChunks.erase(std::remove_if(_loadedChunks.begin(), _loadedChunks.end(),
				     [&](Chunk *chunk)
				     {
				       Vector2i pos(chunk->getPosition());
				       bool	removed;

				       removed = (getClosestPlayer(clients, pos.x, pos.y) >= maxDist);
				       counter += removed;
				       return removed;
				     }), _loadedChunks.end());
  return counter;
}

bool	World::isChunkLoaded(const Vector2i &pos) const
{
  auto it = std::find_if(_loadedChunks.begin(), _loadedChunks.end(),
			 [pos](Chunk *chunk)
			 {
			   return (chunk->getPosition() == pos);
			 });
  return (it != _loadedChunks.end());
}

std::string		World::serialize(const std::vector<Vector2i> &chunkIds)
{
  ProtocolMessage	msg;
  FullChunk		*fullChunk = new FullChunk;
  ChunkData		*chunkData;
  VectorInt		*vecInt;
  std::string		serialized;
  Chunk			*chunk;
  unsigned int		x;
  unsigned int		y;

  for (auto chunkId : chunkIds)
    {
      chunkData = fullChunk->add_chunkdata();

      if (chunkData == nullptr || (vecInt = new VectorInt) == nullptr)
	continue ;
      if ((chunk = getChunk(chunkId)) != nullptr)
	{
	  for (y = 0; y < Chunk::height; ++y)
	    {
	      for (x = 0; x < Chunk::width; ++x)
		{
		  chunkData->add_bgtiles(static_cast<unsigned int>(chunk->getBgTile(x, y)));
		  chunkData->add_fgtiles(static_cast<unsigned int>(chunk->getTile(x, y)));
		}
	    }
	}
      else
	continue ;
      vecInt->set_x(chunkId.x);
      vecInt->set_y(chunkId.y);
      chunkData->set_allocated_id(vecInt);
    }
  msg.set_action(ProtocolMessage::CHUNK);
  msg.set_allocated_fullchunk(fullChunk);
  msg.SerializeToString(&serialized);
  return serialized;
}