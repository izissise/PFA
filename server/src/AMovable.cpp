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

const Vector2i	&AMovable::getPosition() const
{
  return _pos;
}

AMovable::Direction	AMovable::getDirection() const
{
  return _dir;
}
