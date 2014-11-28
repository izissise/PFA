#include "World.hpp"

void World::Camera::translate(const worldPos& v)
{
  _center += v;
  _botLeft.x += v.x;
  _botLeft.y += v.y;
}

void World::Camera::move(const worldPos& pos)
{
  _center = pos;
  _botLeft.x = _center.x - _size.w / 2.0f;
  _botLeft.y = _center.y - _size.h / 2.0f;
}

void World::Camera::scale(const worldPos& s)
{
  _size.w *= s.w;
  _size.h *= s.h;
}

void World::Camera::resize(const worldPos& s)
{
  _size = s;
}
