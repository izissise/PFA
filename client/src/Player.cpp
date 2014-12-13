#include "Player.hpp"

Player::Player(Camera &camera) :
  _camera(camera)
{
}

Player::~Player()
{
}
#include <iostream>
void	Player::move(const Vector2f &dir)
{
  _camera.translate(dir);
  calculateVisibleRange();
  AMovable::move(dir);
  Vector2i mchunkId = getChunkId();
  Vector2f mposition = getPosition();
  std::cout << "Chunk: " << mchunkId.x << " " << mchunkId.y << " "
	    << "Pos : " << mposition.x << " " << mposition.y << std::endl;
}

void	Player::setPlayerPosition(const Vector2i &chunkId,
				  const Vector2f &position)
{
  _camera.move(_camera.sToWPos(chunkId, position));
  calculateVisibleRange();
  _loadedRange =
    {
      {_visibleRange.left() - 1, _visibleRange.bottom() - 1},
      {_visibleRange.right() + 1, _visibleRange.top() + 1}
    };
  AMovable::setChunkId(chunkId);
  AMovable::setPosition(position);
}

Range2i	&Player::getLoadedRange()
{
  return _loadedRange;
}

Range2i	&Player::getVisibleRange()
{
  return _visibleRange;
}

const Range2i	&Player::getVisibleRange() const
{
  return _visibleRange;
}

void	Player::setLoadedRange(const Range2i &range)
{
  _loadedRange = range;
}

void	Player::calculateVisibleRange()
{
  _visibleRange = {Vector2i(std::floor(_camera.left()), std::floor(_camera.bottom())),
		   Vector2i(std::floor(_camera.right()), std::floor(_camera.top()))};
}
