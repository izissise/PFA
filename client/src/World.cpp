#include <ctgmath>
#include <stdexcept>

#include "World.hpp"

World::World(Settings& settings) :
  _settings(settings)
{
  screenPos	screenSize(std::stoi(_settings.getCvarList().getCvar("r_width")),
			   std::stoi(_settings.getCvarList().getCvar("r_height")));
  chunkId	first;
  chunkId	last;

  _camera.resize(_sToWPos(screenSize));
  _camera.move({0.5f, 0.5f});

  _calculateVisibleRange();
  for (auto cursor : _visibleRange) {
    _chunks[cursor] = std::unique_ptr<Chunk>(new Chunk());
    _chunks[cursor]->loadFromFile(cursor.x, cursor.y, _codex);
  }
  _loadedRange = _visibleRange;
}

void World::update(void)
{
  static std::pair<Action, Vector2f> moveControls[] =
    {
      {Action::Forward, {.0f, -.0025f}},
      {Action::Left, {-.0025f, .0f}},
      {Action::Back, {.0f, .0025f}},
      {Action::Right, {.0025f, .0f}}
    };

  for (auto control : moveControls) {
    if (_settings.getControls().getActionState(control.first)) {
      _camera.translate(control.second);
    }
  }
  if (_camera.left() < _visibleRange.left()
      or _camera.top() < _visibleRange.top()
      or _camera.right() > _visibleRange.right()
      or _camera.bottom() > _visibleRange.bottom()) {
    _calculateVisibleRange();
    _loadChunks();
  }
}

void World::draw(sf::RenderWindow& window) const
{
  worldPos	worldOrigin;
  screenPos	screenOrigin;
  screenPos	screenCoord;

  worldOrigin.x = _getGridOffset(_camera.left());
  worldOrigin.y = _getGridOffset(_camera.top());
  screenOrigin = _wToSPos(worldOrigin);
  screenCoord = screenOrigin;
  for (auto cursor : _visibleRange) {
    if (cursor.x == _visibleRange.origin().x and cursor.y != _visibleRange.origin().y) {
      screenCoord.x = screenOrigin.x;
      screenCoord.y += Chunk::height * TileCodex::tileSize;
    }
    _drawChunk(window, cursor, screenCoord);
    screenCoord.x += Chunk::width * TileCodex::tileSize;
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

void World::_calculateVisibleRange(void)
{
  _visibleRange = {Vector2i(floor(_camera.left()), floor(_camera.top())),
		   Vector2i(floor(_camera.right()), floor(_camera.bottom()))};
}

void World::_drawChunk(sf::RenderWindow& window,
		       const chunkId& cursor,
		       screenPos& windowCoord) const
{
  try {
    _chunks.at(cursor)->draw(window, windowCoord, _codex);
  } catch (const std::out_of_range& e) {
    // this means the chunk isn't loaded so we do nothing and skip it
    return ;
  }
}

void World::_loadChunks(void)
{
  Range2i	bufferRange({_visibleRange.left() - 1, _visibleRange.top() - 1},
			    {_visibleRange.right() + 1, _visibleRange.bottom() + 1});

  for (auto cursor : bufferRange) {
    try {
      _chunks.at(cursor);
    } catch (const std::out_of_range& e) {
      _chunks[cursor] = std::unique_ptr<Chunk>(new Chunk());
      _chunks[cursor]->loadFromFile(cursor.x, cursor.y, _codex);
    }
  }
  _loadedRange = bufferRange;
}
