#include "World.hpp"

void World::Camera::translate(const worldPos& v) noexcept
{
  _center += v;
  _topLeft.x += v.x;
  _topLeft.y += v.y;
}

void World::Camera::move(const worldPos& pos) noexcept
{
  _center = pos;
  _topLeft.x = _center.x - _size.w / 2.0f;
  _topLeft.y = _center.y - _size.h / 2.0f;
}

void World::Camera::scale(const worldPos& s) noexcept
{
  _size.w *= s.w;
  _size.h *= s.h;
}

void World::Camera::resize(const worldPos& s) noexcept
{
  _size = s;
}
