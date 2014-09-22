#include <ctgmath>
#include <stdexcept>

#include "World.hpp"

World::World(Settings& set) :
  _set(set),
  _camCenter(0.5f, 0.5f)
{
  _screenSize = {std::stoi(set.getCvarList().getCvar("r_width")),
		 std::stoi(set.getCvarList().getCvar("r_height"))};
  _camSize = _sToWPos(_screenSize);
  _camPos = _camCenter - _camSize / 2.0f;

  std::pair<int, int>	first;
  std::pair<int, int>	last;

  first = {floor(_camPos.x), floor(_camPos.y)};
  last = {floor(_camPos.x + _camSize.x), floor(_camPos.y + _camSize.y)};
  for (auto cursor = first; cursor.second <= last.second; ++cursor.second) {
    for (cursor.first = first.first; cursor.first <= last.first; ++cursor.first) {
      _chunks[cursor] = new Chunk();
      _chunks[cursor]->loadFromFile(cursor.first, cursor.second, _codex);
    }
  }
}

void World::draw(sf::RenderWindow& window) const
{
  std::pair<int, int>	first;
  std::pair<int, int>	last;
  worldPos		worldOrigin;
  screenPos		screenOrigin;
  screenPos		screenCoord;
  auto			getNormOffset = [](float w){
    return -(w - floor(w));
  };

  worldOrigin.x = getNormOffset(_camPos.x);
  worldOrigin.y = getNormOffset(_camPos.y);
  screenOrigin = _wToSPos(worldOrigin);
  screenCoord = screenOrigin;
  first = {floor(_camPos.x), floor(_camPos.y)};
  last = {floor(_camPos.x + _camSize.x), floor(_camPos.y + _camSize.y)};
  for (auto cursor = first; cursor.second <= last.second; ++cursor.second) {
    for (cursor.first = first.first; cursor.first <= last.first; ++cursor.first) {
      _drawChunk(window, cursor, screenCoord);
      screenCoord.x += Chunk::width * TileCodex::tileSize;
    }
    screenCoord.x = screenOrigin.x;
    screenCoord.y += Chunk::height * TileCodex::tileSize;
  }
}

void World::translateCamera(const sf::Vector2<float>& v)
{
  _camCenter += v;
  _camPos.x += v.x;
  _camPos.y += v.y;
}

void World::moveCamera(const sf::Vector2<float>& pos)
{
  _camCenter = pos;
  _camPos.x = _camCenter.x - _camSize.x / 2.0f;
  _camPos.y = _camCenter.y - _camSize.y / 2.0f;
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
