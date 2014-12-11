#include "AMovable.hpp"

AMovable::AMovable()
{
}

AMovable::~AMovable()
{
}

void	AMovable::move(const Vector2f &dir)
{
  _pos.x += dir.x;
  _pos.y += dir.y;
}

const Vector2f	&AMovable::getPosition() const
{
  return _pos;
}

const Vector2i	&AMovable::getChunkId() const
{
  return _chunkId;
}

void	AMovable::setPosition(const Vector2f &position)
{
  _pos = position;
}

void	AMovable::setChunkId(const Vector2i chunkId)
{
  _chunkId = chunkId;
}
