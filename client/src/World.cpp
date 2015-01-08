#include <algorithm>
#include <ctgmath>
#include <stdexcept>

#include "World.hpp"
#include "Perlin.h"

World::World(Settings& settings) :
  _settings(settings),
  _camera(),
  _player(_camera),
  _loaded(false)
{
  CvarList	&cvarList = _settings.getCvarList();
  chunkId	first;
  chunkId	last;

  _screenSize =	{std::stoi(cvarList.getCvar("r_width")),
		 std::stoi(cvarList.getCvar("r_height"))};
  _camera.resize(_camera.sToWPos(_screenSize));
}

void		World::load()
{
  loadRange();
  _loaded = true;
}

void		World::loadRange()
{
  Range2i	&loadedRange = _player.getLoadedRange();

  std::cout << " -- LoadRange -- " << std::endl;
  for (auto &cursor : loadedRange)
    {
      std::cout << cursor.x << " " << cursor.y << std::endl;
      _chunks[cursor]->load(_codex);
    }
  std::cout << "-----" << std::endl;
}

void		World::setPlayerPosition(const Vector2i &chunkId,
					 const Vector2f &position)
{
  Range2i	loadedRange;

  _player.setPlayerPosition(chunkId, position,
			    std::stof(_settings.getCvarList().getCvar("r_width")),
			    std::stof(_settings.getCvarList().getCvar("r_height")));
  loadedRange = _player.getLoadedRange();
  for (const auto &cursor : loadedRange)
    {
      _chunks[cursor] = std::unique_ptr<Chunk>(new Chunk());
      _chunks[cursor]->setPosition({cursor.x, cursor.y});
    }
}

void   	World::fillChunkData(const VectorInt &pos,
			     const RepeatedField<uint32> &bgTiles,
			     const RepeatedField<uint32> &fgTiles)
{
  auto	chunk = _chunks.find(Vector2i(pos.x(), pos.y()));

  if (chunk == _chunks.end())
    {
      std::cout << "Could not find Chunks at pos " << pos.x() << " " << pos.y() << std::endl;
      return ;
    }
  (chunk->second)->fillTiles(bgTiles, fgTiles);
}

bool		World::movePlayer(const Vector2f &dir)
{
  bool		retVal;

  retVal = _player.move(dir);
  if (retVal)
    _loadChunks();
  return retVal;
}

bool		World::movePlayer(const VectorInt &chunkId,
				  const VectorFloat &pos)
{
  return movePlayer(_camera.sToWPos(Vector2i(chunkId.x(), chunkId.y()),
				    Vector2f(pos.x(), pos.y()))
		    - _camera.center());
}

void	World::update()
{
}

auto World::_getScreenOrigin(void) const -> screenPos
{
  worldPos worldOrigin;

  worldOrigin.x = -(_camera.left() - std::floor(_camera.left()));
  worldOrigin.y = -(1 - (_camera.top() - std::floor(_camera.top())));
  return _camera.wToSPos(worldOrigin);
}

void	World::draw(sf::RenderTarget &window) const
{
  screenPos	screenOrigin = _getScreenOrigin();
  screenPos	screenCoord = screenOrigin;
  const Range2i	&range = _player.getVisibleRange();
  int		x;

  if (!_loaded)
    return ;
  for (int y = range.top(); y >= range.bottom(); --y) {
    for (x = range.left(); x <= range.right(); ++x) {
      _drawChunk(window, {x, y}, screenCoord);
      screenCoord.x += Chunk::width * TileCodex::tileSize;
    }
    screenCoord.x = screenOrigin.x;
    screenCoord.y += Chunk::height * TileCodex::tileSize;
  }
}

float World::_getGridOffset(float w) const
{
  return -(w - std::floor(w));
}

void World::_drawChunk(sf::RenderTarget& window,
		       const chunkId& cursor,
		       screenPos& windowCoord) const
{
  try {
    const Chunk& target = *_chunks.at(cursor);
    if (target.isLoaded())
      target.draw(window, windowCoord, _codex);
  } catch (const std::out_of_range& e) {
    // this means the chunk isn't loaded so we do nothing and skip it
    return ;
  }
}

/*
** This method probably needs refactoring. It doesn't seem very optimal
** even though for now it works and that's currently good enough.
*/
void		World::_loadChunks(void)
{
  const Range2i		&visibleRange = _player.getVisibleRange();
  Range2i		&loadedRange = _player.getLoadedRange(); // should be const
  const	Vector2i	plChunkId = _player.getChunkId();
  Vector2i		sideSize;
  Vector2i		res(std::stoi(_settings.getCvarList().getCvar("r_width")),
			    std::stoi(_settings.getCvarList().getCvar("r_height")));
  std::function<int (int num, int factor)> roundFunc = [](int num, int factor)
    -> int
    {
      return (num == 0 ? 0 : num + factor - 1 - (num - 1) % factor);
    };

  sideSize.x = 2 + roundFunc(res.x / Chunk::pWidth, 2) + 1;
  sideSize.y = 2 + roundFunc(res.y / Chunk::pHeight, 2) + 1;
  removeOldChunks();
  _player.setLoadedRange(Range2i({plChunkId.x - (sideSize.x - 1) / 2,
				plChunkId.y - (sideSize.y - 1) / 2},
				{plChunkId.x + (sideSize.x - 1) / 2,
				 plChunkId.y + (sideSize.y - 1) / 2}));
  for (auto cursor : loadedRange) // be aware it has been updated just above
    if (!isChunkLoaded(cursor))
      _chunks.emplace(cursor, std::unique_ptr<Chunk>(new Chunk(cursor)));
}

const Player	&World::getPlayer() const
{
  return _player;
}

bool	World::isChunkLoaded(const Vector2i &chunkPos) const
{
  auto	it = _chunks.find(chunkPos);

  if (it == _chunks.end())
    return false;
  return (it->second->isGenerated() || it->second->isLoaded());
}

void			World::removeOldChunks()
{
  return ;
  const Vector2i	&chunkPos = _player.getChunkId();
  auto			itr = _chunks.begin();
  unsigned int		radius = 1 + World::cacheSize;

  while (itr != _chunks.end())
    {
      const Vector2i &pos = itr->second->getPosition();
      if (itr->second->isGenerated() &&
	  (std::abs(chunkPos.x - pos.x) > radius ||
	   std::abs(chunkPos.y - pos.y) > radius))
	{
	  std::cout << "Removing chunk at pos " << pos.x << " " << pos.y << std::endl;
	  _chunks.erase(itr++);
	}
      else
	++itr;
    }
}

bool			World::getNewChunks(std::vector<Vector2i> &chunks)
{
  const Vector2i	&chunkPos = _player.getChunkId();
  Vector2u		sideSize;
  Vector2i		res(std::stoi(_settings.getCvarList().getCvar("r_width")),
			    std::stoi(_settings.getCvarList().getCvar("r_height")));
  float			rWidth;
  float			rHeight;

  std::cout << "Player idpos: " << chunkPos.x << " " << chunkPos.y << std::endl;
  // +1 is the Center, X * 2 for what is bordering it, + 2 for the sides
  sideSize.x = 2 + std::ceil(res.x / Chunk::pWidth) + 1;
  sideSize.y = 2 + std::ceil(res.y / Chunk::pHeight) + 1;

  for (int y = chunkPos.y - (sideSize.y - 1) / 2;
       y <= chunkPos.y + (static_cast<int>(sideSize.y) - 1) / 2; ++y)
    {
      for (int x = chunkPos.x - (sideSize.x - 1) / 2;
  	   x <= chunkPos.x + (static_cast<int>(sideSize.x) - 1) / 2; ++x)
	{
	  if (!isChunkLoaded({x, y}))
	    chunks.push_back({x, y});
	}
    }
  return !chunks.empty();
}
