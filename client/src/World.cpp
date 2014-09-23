#include <ctgmath>
#include <stdexcept>

#include "World.hpp"

World::World(Settings& set) :
  _set(set),
  _screenSize(std::stoi(set.getCvarList().getCvar("r_width")),
	      std::stoi(set.getCvarList().getCvar("r_height")))
{
  chunkId	first;
  chunkId	last;

  camera.resize(_sToWPos(_screenSize));
  camera.move({0.5f, 0.5f});

  first = {floor(camera.left()), floor(camera.top())};
  last = {floor(camera.right()), floor(camera.bottom())};
  for (auto cursor = first; cursor.second <= last.second; ++cursor.second) {
    for (cursor.first = first.first; cursor.first <= last.first; ++cursor.first) {
      _chunks[cursor] = new Chunk();
      _chunks[cursor]->loadFromFile(cursor.first, cursor.second, _codex);
    }
  }
}

void World::draw(sf::RenderWindow& window) const
{
  chunkId	first;
  chunkId	last;
  worldPos	worldOrigin;
  screenPos	screenOrigin;
  screenPos	screenCoord;

  worldOrigin.x = _getGridOffset(camera.left());
  worldOrigin.y = _getGridOffset(camera.top());
  screenOrigin = _wToSPos(worldOrigin);
  screenCoord = screenOrigin;
  first = {floor(camera.left()), floor(camera.top())};
  last = {floor(camera.right()), floor(camera.bottom())};
  for (auto cursor = first; cursor.second <= last.second; ++cursor.second) {
    for (cursor.first = first.first; cursor.first <= last.first; ++cursor.first) {
      _drawChunk(window, cursor, screenCoord);
      screenCoord.x += Chunk::width * TileCodex::tileSize;
    }
    screenCoord.x = screenOrigin.x;
    screenCoord.y += Chunk::height * TileCodex::tileSize;
  }
}

auto World::_sToWPos(screenPos pos) const -> worldPos
{
  return {static_cast<float>(pos.x) / (Chunk::width * TileCodex::tileSize),
      static_cast<float>(pos.y) / (Chunk::height * TileCodex::tileSize)};
}

auto World::_wToSPos(worldPos pos) const -> screenPos
{
  return {static_cast<int>(pos.x * (Chunk::width * TileCodex::tileSize)),
      static_cast<int>(pos.y * (Chunk::height * TileCodex::tileSize))};
}

float World::_getGridOffset(float w) const
{
    return -(w - floor(w));
}

void World::_drawChunk(sf::RenderWindow& window,
		       const chunkId& chunkCursor,
		       sf::Vector2<int>& windowCoord) const
{
  Chunk	*chunk;

  try {
    chunk = _chunks.at(chunkCursor);
  } catch (const std::out_of_range& e) {
    // this means the chunk isn't loaded so we do nothing and skip it
    return ;
  }
  chunk->draw(window, windowCoord, _codex);
}
