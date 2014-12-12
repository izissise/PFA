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
  Range2i	&loadedRange = _player.getLoadedRange();

  for (auto &cursor : loadedRange)
    _chunks[cursor]->load(_codex);
  _loaded = true;
}

void		World::setPlayerPosition(const Vector2i &chunkId,
					 const Vector2f &position)
{
  Range2i	loadedRange;

  _player.setPlayerPosition(chunkId, position);
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
      std::cout << "Could not find Chunks at pos y: " << pos.y() << " x: " << pos.x() << std::endl;
      return ;
    }
  (chunk->second)->fillTiles(bgTiles, fgTiles);
}

void		World::movePlayer(const Vector2f &dir)
{
  Range2i	oldRange = _player.getVisibleRange();
  Range2i	visibleRange;

  _player.move(dir);
  visibleRange = _player.getVisibleRange();
  if (visibleRange != oldRange)
    _loadChunks();
}

void		World::movePlayer(const VectorInt &chunkId,
				  const VectorFloat &pos)
{
  movePlayer(_camera.sToWPos(Vector2i(chunkId.x(), chunkId.y()),
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

void	World::draw(sf::RenderWindow& window) const
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

void World::_drawChunk(sf::RenderWindow& window,
		       const chunkId& cursor,
		       screenPos& windowCoord) const
{
  try {
    const Chunk& target = *_chunks.at(cursor);
    if (target.isLoaded()) {
      target.draw(window, windowCoord, _codex);
    }
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
  const Range2i	&visibleRange = _player.getVisibleRange();
  const Range2i	&loadedRange = _player.getLoadedRange();
  Range2i	bufferRange({visibleRange.left() - 1, visibleRange.bottom() - 1},
			    {visibleRange.right() + 1, visibleRange.top() + 1});

  std::vector<Vector2i>	added;
  std::vector<Vector2i> removed;
  Range2i	intersection =
    {
      {std::max(bufferRange.left(), loadedRange.left()),
       std::max(bufferRange.bottom(), loadedRange.bottom())},
      {std::min(bufferRange.right(), loadedRange.right()),
       std::min(bufferRange.top(), loadedRange.top())}
    };
  auto		predicate = [intersection](Vector2i& v) {
    return (std::find(intersection.cbegin(), intersection.cend(), v) != intersection.cend());
  };

  added.assign(bufferRange.begin(), bufferRange.end());
  removed.assign(loadedRange.cbegin(), loadedRange.cend());
  added.erase(std::remove_if(added.begin(), added.end(), predicate), added.end());
  removed.erase(std::remove_if(removed.begin(), removed.end(), predicate), removed.end());

  for (auto cursor : added)
    {
      if (_chunks.find(cursor) == _chunks.end())
	{
	  _chunks.emplace(cursor, std::unique_ptr<Chunk>(new Chunk()));
	  _chunks[cursor]->load(_codex);
	}
    }
  for (auto cursor : removed)
    _chunks.erase(cursor);
  _player.setLoadedRange(bufferRange);
}

const Player	&World::getPlayer() const
{
  return _player;
}
