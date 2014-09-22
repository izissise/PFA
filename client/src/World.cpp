#include <ctgmath>
#include <stdexcept>

#include "World.hpp"

World::World(Settings& set) :
  _set(set),
  _center(0.0f, 0.0f)
{
  _box.width = std::stof(set.getCvarList().getCvar("r_width")) / (Chunk::width * TileCodex::tileSize);
  _box.height = std::stof(set.getCvarList().getCvar("r_height")) / (Chunk::height * TileCodex::tileSize);
  _box.left = -(_box.width / 2.0f);
  _box.top = -(_box.height / 2.0f);

  std::pair<int, int>	firstChunk;
  std::pair<int, int>	lastChunk;

  firstChunk.first = roundf(_box.left);
  firstChunk.second = roundf(_box.top);
  lastChunk.first = roundf(_box.left + _box.width);
  lastChunk.second = roundf(_box.top + _box.height);
  for (auto chunkCursor = firstChunk;
       chunkCursor.second <= lastChunk.second;
       ++chunkCursor.second) {
    for (chunkCursor.first = firstChunk.first;
	 chunkCursor.first <= lastChunk.first;
	 ++chunkCursor.first) {
      _chunks[chunkCursor] = new Chunk();
      _chunks[chunkCursor]->loadFromFile(chunkCursor.first, chunkCursor.second, _codex);
    }
  }
}

void World::draw(sf::RenderWindow& window) const
{
  std::pair<int, int>	firstChunk;
  std::pair<int, int>	lastChunk;
  sf::Vector2<int>	windowOrigin(0, 0);
  sf::Vector2<int>	windowCoord(0, 0);

  //TODO appropriate calculations to initialize windowCoord
  double intpart;
  //std::cout << _box.left << std::endl << _box.top << _box.width << std::endl << _box.height << std::endl << std::endl;
  windowOrigin.x = (modf(_box.left, &intpart) ? _box.left - ceil(_box.left) : 0) * Chunk::width * TileCodex::tileSize;
  windowOrigin.y = (modf(_box.top, &intpart) ? _box.top - ceil(_box.top) : 0) * Chunk::height * TileCodex::tileSize;
  windowCoord = windowOrigin;
  //std::cout << windowOrigin.x << std::endl << windowOrigin.y << std::endl << _box.left << std::endl << _box.top << std::endl;
  firstChunk.first = roundf(_box.left);
  firstChunk.second = roundf(_box.top);
  lastChunk.first = roundf(_box.left + _box.width);
  lastChunk.second = roundf(_box.top + _box.height);
  //std::cout << firstChunk.first << std::endl << firstChunk.second << std::endl << lastChunk.first << std::endl << lastChunk.second << std::endl;
  for (auto chunkCursor = firstChunk;
       chunkCursor.second <= lastChunk.second;
       ++chunkCursor.second) {
    for (chunkCursor.first = firstChunk.first;
	 chunkCursor.first <= lastChunk.first;
	 ++chunkCursor.first) {
      _drawChunk(window, chunkCursor, windowCoord);
      windowCoord.x += Chunk::width * TileCodex::tileSize;
    }
    windowCoord.x = windowOrigin.x;
    windowCoord.y += Chunk::height * TileCodex::tileSize;
  }
}

void World::translateCamera(const sf::Vector2<float>& v)
{
  _center += v;
  _box.left += v.x;
  _box.top += v.y;
}

void World::moveCamera(const sf::Vector2<float>& pos)
{
  _center = pos;
  _box.left = _center.x - _box.width / 2.0f;
  _box.top = _center.y - _box.height / 2.0f;
}

Chunk* World::_getChunk(int x, int y)
{
  Chunk* chunk = _chunks[std::make_pair(x, y)];

  if (not chunk->isLoaded()) {
    return nullptr;
  }
  return chunk;
}


void World::_drawChunk(sf::RenderWindow& window,
		       const std::pair<int, int>& chunkCursor,
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
