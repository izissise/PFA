#include <algorithm>
#include <ctgmath>
#include <stdexcept>

#include "World.hpp"

World::World(Settings& settings) :
  _settings(settings)
{
  chunkId	first;
  chunkId	last;

  _screenSize =	{std::stoi(_settings.getCvarList().getCvar("r_width")),
		 std::stoi(_settings.getCvarList().getCvar("r_height"))};
  _camera.resize(_sToWPos(_screenSize, true));
  _camera.move({0.5f, 0.5f});

  _calculateVisibleRange();
  Range2i bufferRange =
    {
      {_visibleRange.left() - 1, _visibleRange.bottom() - 1},
      {_visibleRange.right() + 1, _visibleRange.top() + 1}
    };
  for (auto cursor : bufferRange) {
    _chunks[cursor] = std::unique_ptr<Chunk>(new Chunk());
    _chunks[cursor]->load(cursor.x, cursor.y, _codex);
  }
  _loadedRange = bufferRange;
}

void World::update(void)
{
  static std::pair<Action, Vector2f> moveControls[] =
    {
      {Action::Forward, {.0f, .025f}},
      {Action::Left, {-.025f, .0f}},
      {Action::Back, {.0f, -.025f}},
      {Action::Right, {.025f, .0f}}
    };

  for (auto control : moveControls) {
    if (_settings.getControls().getActionState(control.first)) {
      _camera.translate(control.second);
    }
  }
  Range2i oldRange = _visibleRange;

  _calculateVisibleRange();
  if (_visibleRange != oldRange) {
    _loadChunks();
  }
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
    return (std::find(intersection.begin(), intersection.end(), v) != intersection.end());
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
