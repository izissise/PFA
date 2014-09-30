#include <algorithm>
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
  Range2i bufferRange = {{_visibleRange.left() - 1, _visibleRange.top() - 1},
			 {_visibleRange.right() + 1, _visibleRange.bottom() + 1}};
  for (auto cursor : bufferRange) {
    _chunks[cursor] = std::unique_ptr<Chunk>(new Chunk());
    _chunks[cursor]->loadFromFile(cursor.x, cursor.y, _codex);
  }
  _loadedRange = bufferRange;
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
      or _camera.right() > _visibleRange.right() + 1
      or _camera.bottom() > _visibleRange.bottom() + 1) {
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
    const Chunk& target = *_chunks.at(cursor);
    if (target.isLoaded()) {
      target.draw(window, windowCoord, _codex);
    }
  } catch (const std::out_of_range& e) {
    // this means the chunk isn't loaded so we do nothing and skip it
    return ;
  }
}

void World::_loadChunks(void)
{
  Range2i	bufferRange({_visibleRange.left() - 1, _visibleRange.top() - 1},
			    {_visibleRange.right() + 1, _visibleRange.bottom() + 1});

  std::vector<Vector2i>	added;
  std::vector<Vector2i> removed;

  for (auto br : bufferRange) {
    added.push_back(br);
  }
  for (auto lr : _loadedRange) {
    removed.push_back(lr);
  }

  Range2i	intersection =
    {
      {std::max(bufferRange.left(), _loadedRange.left()),
       std::max(bufferRange.top(), _loadedRange.top())},
      {std::min(bufferRange.right(), _loadedRange.right()),
       std::min(bufferRange.bottom(), _loadedRange.bottom())}
    };
  auto		predicate = [intersection](Vector2i& v) {
    return (std::find(intersection.begin(), intersection.end(), v) != intersection.end());
  };
  added.erase(std::remove_if(added.begin(), added.end(), predicate), added.end());
  removed.erase(std::remove_if(removed.begin(), removed.end(), predicate), removed.end());

  // std::cout << std::endl
  // 	    << "CAMERA" << std::endl
  // 	    << "-------" << std::endl
  // 	    << "left: " << _camera.left() << std::endl
  // 	    << "top: " << _camera.top() << std::endl
  // 	    << "right: " << _camera.right() << std::endl
  // 	    << "bottom: " << _camera.bottom() << std::endl
  // 	    << std::endl
  // 	    << "VISIBLE" << std::endl
  // 	    << "-------" << std::endl
  // 	    << "left: " << _visibleRange.left() << std::endl
  // 	    << "top: " << _visibleRange.top() << std::endl
  // 	    << "right: " << _visibleRange.right() << std::endl
  // 	    << "bottom: " << _visibleRange.bottom() << std::endl;

  for (auto cursor : added) {
    try {
      _chunks.at(cursor);
    } catch (const std::out_of_range& e) {
      _chunks.emplace(cursor, std::unique_ptr<Chunk>(new Chunk()));
      _chunks[cursor]->loadFromFile(cursor.x, cursor.y, _codex);
    }
  }
  for (auto cursor : removed) {
    auto chunk = _chunks.find(cursor);
    if (chunk != _chunks.end()) {
      _chunks.erase(chunk);
    }
  }
}
