#include <algorithm>
#include <ctgmath>
#include <stdexcept>

#include "World.hpp"
#include "Perlin.h"

World::World(Settings& settings) :
  _settings(settings)
{
  CvarList	&cvarList = _settings.getCvarList();
  chunkId	first;
  chunkId	last;

  _screenSize =	{std::stoi(cvarList.getCvar("r_width")),
		 std::stoi(cvarList.getCvar("r_height"))};
  _camera.resize(_sToWPos(_screenSize, true));
  _camera.move({0.5f, 0.5f});
  _visibleRange = {Vector2i(0,0),
  		   Vector2i(0,0)};
  _loadedRange =
    {
      {_visibleRange.left(), _visibleRange.bottom() - 1},
      {_visibleRange.right() + 1, _visibleRange.top() + 1}
    };
}

void	World::setPlayerPosition(const Vector2f &position)
{
  _camera.move(position);
  _calculateVisibleRange();
  _loadedRange =
    {
      {_visibleRange.left(), _visibleRange.bottom() - 1.f},
      {_visibleRange.right() + 1.f, _visibleRange.top() + 1.f}
    };
  for (auto cursor : _loadedRange) {
    _chunks[cursor] = std::unique_ptr<Chunk>(new Chunk());
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
  (chunk->second)->fillTiles(fgTiles, bgTiles);
}

void		World::movePlayer(const Vector2f &dir)
{
  Range2i	oldRange = _visibleRange;

  _camera.translate(dir);
  _calculateVisibleRange();
  if (_visibleRange != oldRange) {
    _loadChunks();
  }
}

void	World::forceChunkReloading()
{
  _loadChunks();
}

void	World::update()
{
}

auto World::_getScreenOrigin(void) const -> screenPos
{
  worldPos worldOrigin;

  worldOrigin.x = -(_camera.left() - floor(_camera.left()));
  worldOrigin.y = -(1 - (_camera.top() - floor(_camera.top())));
  return _wToSPos(worldOrigin, true);
}

void World::draw(sf::RenderWindow& window) const
{
  screenPos	screenOrigin = _getScreenOrigin();
  screenPos	screenCoord = screenOrigin;
  auto&		range = _visibleRange;
  int		x;

  for (int y = range.top(); y >= range.bottom(); --y) {
    for (x = range.left(); x <= range.right(); ++x) {
      _drawChunk(window, {x, y}, screenCoord);
      screenCoord.x += Chunk::width * TileCodex::tileSize;
    }
    screenCoord.x = screenOrigin.x;
    screenCoord.y += Chunk::height * TileCodex::tileSize;
  }
}

auto World::_sToWPos(screenPos pos, bool noOffsets) const -> worldPos
{
  float sy = (noOffsets ? pos.y : _screenSize.y - pos.y);
  float wy = sy / (Chunk::height * TileCodex::tileSize);

  return {static_cast<float>(pos.x) / (Chunk::width * TileCodex::tileSize), wy};
}

auto World::_wToSPos(worldPos pos, bool noOffsets) const -> screenPos
{
  float wy = pos.y * (Chunk::height * TileCodex::tileSize);
  int sy = (noOffsets ? wy : _screenSize.y - wy);

  return {static_cast<int>(pos.x * (Chunk::width * TileCodex::tileSize)), sy};
}

float World::_getGridOffset(float w) const
{
  return -(w - floor(w));
}

void World::_calculateVisibleRange(void)
{
  _visibleRange = {Vector2i(floor(_camera.left()), floor(_camera.bottom())),
		   Vector2i(floor(_camera.right()), floor(_camera.top()))};
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
void World::_loadChunks(void)
{
  Range2i	bufferRange({_visibleRange.left() - 1, _visibleRange.bottom() - 1},
			    {_visibleRange.right() + 1, _visibleRange.top() + 1});

  std::vector<Vector2i>	added;
  std::vector<Vector2i> removed;
  Range2i	intersection =
    {
      {std::max(bufferRange.left(), _loadedRange.left()),
       std::max(bufferRange.bottom(), _loadedRange.bottom())},
      {std::min(bufferRange.right(), _loadedRange.right()),
       std::min(bufferRange.top(), _loadedRange.top())}
    };
  auto		predicate = [intersection](Vector2i& v) {
    return (std::find(intersection.cbegin(), intersection.cend(), v) != intersection.cend());
  };

  added.assign(bufferRange.begin(), bufferRange.end());
  removed.assign(_loadedRange.begin(), _loadedRange.end());
  added.erase(std::remove_if(added.begin(), added.end(), predicate), added.end());
  removed.erase(std::remove_if(removed.begin(), removed.end(), predicate), removed.end());

  for (auto cursor : added) {
    if (_chunks.find(cursor) == _chunks.end()) {
      _chunks.emplace(cursor, std::unique_ptr<Chunk>(new Chunk()));
      _chunks[cursor]->load(cursor.x, cursor.y, _codex);
    }
  }
  for (auto cursor : removed) {
    _chunks.erase(cursor);
  }
  _loadedRange = bufferRange;
}
