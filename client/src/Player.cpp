#include <functional>
#include <iostream>

#include "Player.hpp"

Player::Player(std::shared_ptr<b2World> const& world, Camera &camera) :
  AEntity(world), _camera(camera)
{
}

void Player::update(std::chrono::milliseconds const & timeStep)
{
  AMovable::update(timeStep);
}

/*
void Player::draw(sf::RenderTarget& window, std::chrono::milliseconds const & timeStep) const
{
  AMovable::draw(window, timeStep);
}
*/

bool	Player::move(const Vector2f &dir)
{
  bool	retVal;

  _camera.translate(dir);
  calculateVisibleRange();
  retVal = AMovable::move(dir);
  Vector2i mchunkId = getChunkId();
  Vector2f mposition = getPosition();
  std::cout << "Chunk: " << mchunkId.x << " " << mchunkId.y << " "
	    << "Pos : " << mposition.x << " " << mposition.y << std::endl;
  return retVal;
}

void	Player::setPlayerPosition(const Vector2i &chunkId,
				  const Vector2f &position,
				  float rWidth, float rHeight)
{
  Vector2i	sideSize;
  std::function<int (int num, int factor)> roundFunc = [](int num, int factor)
    -> int
    {
      return (num == 0 ? 0 : num + factor - 1 - (num - 1) % factor);
    };

  sideSize.x = 2 + roundFunc(rWidth / Chunk::pWidth, 2) + 1;
  sideSize.y = 2 + roundFunc(rHeight / Chunk::pHeight, 2) + 1;
  _camera.move(_camera.sToWPos(chunkId, position));
  calculateVisibleRange();
  _loadedRange =
    {
      {chunkId.x - (sideSize.x - 1) / 2, chunkId.y - (sideSize.y - 1) / 2},
      {chunkId.x + (sideSize.x - 1) / 2, chunkId.y + (sideSize.y - 1) / 2}
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
  std::cout << "SetLoaded: " << _loadedRange.left() << " " << _loadedRange.top() << " -> "
	    << _loadedRange.right() << " " << _loadedRange.bottom() << std::endl;
}

void	Player::calculateVisibleRange()
{
  _visibleRange = {Vector2i(std::floor(_camera.left()), std::floor(_camera.bottom())),
		   Vector2i(std::floor(_camera.right()), std::floor(_camera.top()))};
}
