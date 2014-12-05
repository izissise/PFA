#include "AMovable.hpp"

AMovable::AMovable()
{
}

AMovable::~AMovable()
{
}

void	AMovable::move(AMovable::Direction dir)
{
  _dir = dir;
  if (dir == Up || dir == Down)
    _pos.y += ((dir == Up) ? 1 : -1);
  else
    _pos.x += ((dir == Right) ? 1 : -1);
}

const Vector2f	&AMovable::getPosition() const
{
  return _pos;
}

const Vector2i	&AMovable::getChunkId() const
{
  return _chunkId;
}

AMovable::Direction	AMovable::getDirection() const
{
  return _dir;
}

void	AMovable::setPosition(const Vector2f &position)
{
  _pos = position;
}

void	AMovable::setChunkId(const Vector2i chunkId)
{
  _chunkId = chunkId;
}
