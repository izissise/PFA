#include <algorithm>
#include "World.hpp"
#include "Perlin.h"
#include "FastMath.h"

const float World::removeDist = 1024.f; // keep X tiles around him

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

Chunk	*World::loadChunk(const Vector2i &entId,
			  const std::vector<Client *> &clients,
			  int x, int y)
{
  Chunk	*chunk = new Chunk;

  chunk->load(x, y);
  _loadedChunks.push_back(chunk);
  removeUnusedChunks(entId, clients);
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
      if (!cl->isInitialized())
	continue ;
      clEnt = cl->getEntity();
      pos = clEnt.getPosition();
      chunkId = clEnt.getChunkId();
      chunkId.x *= Chunk::width;
      chunkId.y *= Chunk::height;
      pos = Vector2f(chunkId.x + pos.x * Chunk::width,
		     chunkId.y - pos.y * Chunk::height);
      distance = pointDist(std::abs(pos.x - x), std::abs(pos.y - y));
      if (closest == -1 || distance < closest)
	closest = distance;
    }
  return closest;
}

unsigned int	World::removeUnusedChunks(const std::vector<Client *> &clients)
{
  unsigned int	counter = 0;

  _loadedChunks.erase(std::remove_if(_loadedChunks.begin(), _loadedChunks.end(),
				     [&](Chunk *chunk)
				     {
				       Vector2i pos(chunk->getPosition());
				       bool	removed;

				       removed = (getClosestPlayer
						  (clients,
						   pos.x * Chunk::width,
						   pos.y * Chunk::height) >= World::removeDist);
				       counter += removed;
				       if (removed)
					 std::cout << "remove " << pos.x << " " << pos.y << std::endl;
				       return removed;
				     }), _loadedChunks.end());
  return counter;
}

unsigned int	World::removeUnusedChunks(const Vector2i &entId,
					  const std::vector<Client *> &clients)
{
  unsigned int	counter = 0;

  _loadedChunks.erase(std::remove_if(_loadedChunks.begin(), _loadedChunks.end(),
				     [&](Chunk *chunk)
				     {
				       Vector2i pos(chunk->getPosition());
				       bool	removed;
				       float	distance;

				       distance = pointDist(std::abs(pos.x - entId.x) * Chunk::width,
							    std::abs(pos.y - entId.y) * Chunk::height);
				       distance = std::min(getClosestPlayer(clients,
									    pos.x * Chunk::width,
									    pos.y * Chunk::height),
							   distance);
				       removed = (distance >= World::removeDist);
				       counter += removed;
				       if (removed)
					 std::cout << "remove " << pos.x << " " << pos.y << std::endl;
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

std::string		World::serialize(const Vector2i &chunkId)
{
  ProtocolMessage	msg;
  ChunkData		*chunkData = new ChunkData;
  VectorInt		*vecInt;
  std::string		serialized;
  Chunk			*chunk;
  unsigned int		fgcont = 0;
  unsigned int		bgcont = 0;
  TileType		fgtile;
  TileType		bgtile;
  TileType		oldfgtile;
  TileType		oldbgtile;

  if ((chunk = getChunk(chunkId)) != nullptr)
    {
      for (unsigned int y = 0; y < Chunk::height; ++y)
	{
	  for (unsigned int x = 0; x < Chunk::width; ++x)
	    {
	      fgtile = chunk->getTile(x, y);
	      bgtile = chunk->getBgTile(x, y);
	      if (x == 0 && y == 0)
		{
		  oldfgtile = fgtile;
		  oldbgtile = bgtile;
		}
	      else
		{
		  if (fgtile != oldfgtile)
		    {
		      chunkData->add_fgnumber(fgcont);
		      chunkData->add_fgtilecode(static_cast<unsigned int>(oldfgtile));
		      fgcont = 0;
		      oldfgtile = fgtile;
		    }
		  if (bgtile != oldbgtile)
		    {
		      chunkData->add_bgnumber(bgcont);
		      chunkData->add_bgtilecode(static_cast<unsigned int>(oldbgtile));
		      bgcont = 0;
		      oldbgtile = bgtile;
		    }
		}
	      ++fgcont;
	      ++bgcont;
	    }
	}
    }
  chunkData->add_bgnumber(bgcont);
  chunkData->add_bgtilecode(static_cast<unsigned int>(oldbgtile));
  chunkData->add_fgnumber(fgcont);
  chunkData->add_fgtilecode(static_cast<unsigned int>(oldfgtile));
  vecInt = new VectorInt;
  vecInt->set_x(chunkId.x);
  vecInt->set_y(chunkId.y);
  chunkData->set_allocated_id(vecInt);
  msg.set_content(ProtocolMessage::CHUNK);
  msg.set_allocated_chunkdata(chunkData);
  msg.SerializeToString(&serialized);
  return serialized;
}

std::string		World::serialize(const std::vector<Vector2i> &chunkIds,
					 const std::vector<Vector2i> &positions,
					 const std::vector<Vector2i> &sizes)
{
  ProtocolMessage	msg;
  WorldZone		*worldZone = new WorldZone;
  ChunkZone		*chunkZone;
  VectorInt		*id;
  VectorUint		*position;
  VectorUint		*size;
  std::string		serialized;

  Chunk			*chunk;
  unsigned int		x;
  unsigned int		y;
  unsigned int		chunkNb;
  unsigned int		nbChunks = chunkIds.size();

  for (chunkNb = 0; chunkNb < nbChunks; ++chunkNb)
    {
      if ((chunk = getChunk(chunkIds[chunkNb])) != nullptr)
  	{
	  chunkZone = worldZone->add_chunkzone();

	  if (chunkZone == nullptr)
	    continue ;
  	  for (y = 0; y < Chunk::height; ++y)
  	    {
  	      for (x = 0; x < Chunk::width; ++x)
  		{
  		  chunkZone->add_fgtiles(static_cast<unsigned int>(chunk->getTile(x, y)));
  		}
  	    }
  	}
      else
  	continue ;
      id = new VectorInt;
      position = new VectorUint;
      size = new VectorUint;

      id->set_x(chunkIds[chunkNb].x);
      id->set_y(chunkIds[chunkNb].y);
      position->set_x(positions[chunkNb].x);
      position->set_y(positions[chunkNb].y);
      size->set_x(sizes[chunkNb].x);
      size->set_y(sizes[chunkNb].y);
      chunkZone->set_allocated_id(id);
      chunkZone->set_allocated_position(position);
      chunkZone->set_allocated_size(size);
    }
  msg.set_content(ProtocolMessage::CHUNKZONE);
  msg.set_allocated_worldzone(worldZone);
  msg.SerializeToString(&serialized);
  return serialized;
}
