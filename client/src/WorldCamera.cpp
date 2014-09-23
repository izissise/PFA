#include "World.hpp"

void World::Camera::translate(const sf::Vector2<float>& v) noexcept
{
  _center += v;
  _topLeft.x += v.x;
  _topLeft.y += v.y;
}

void World::Camera::move(const sf::Vector2<float>& pos) noexcept
{
  _center = pos;
  _topLeft.x = _center.x - _size.x / 2.0f;
  _topLeft.y = _center.y - _size.y / 2.0f;
}

void World::Camera::scale(const sf::Vector2<float>& s) noexcept
{
  _size.x *= s.x;
  _size.y *= s.y;
}

void World::Camera::resize(const sf::Vector2<float>& s) noexcept
{
  _size = s;
}
