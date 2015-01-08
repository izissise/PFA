#include "Camera.hpp"

void Camera::translate(const worldPos& v)
{
  _center += v;
  _botLeft.x += v.x;
  _botLeft.y += v.y;
}

void Camera::move(const worldPos& pos)
{
  _center = pos;
  _botLeft.x = _center.x - _size.w / 2.f;
  _botLeft.y = _center.y - _size.h / 2.f;
}

void Camera::scale(const worldPos& s)
{
  _size.w *= s.w;
  _size.h *= s.h;
}

void Camera::resize(const worldPos& s)
{
  _size = s;
}

auto	Camera::wToSPos(const worldPos &pos) const -> screenPos
{
  return {static_cast<int>(pos.x * static_cast<float>(Chunk::pWidth)),
      static_cast<int>(pos.y * static_cast<float>(Chunk::pHeight))};
}

auto	Camera::sToWPos(const screenPos &pos) const -> worldPos
{
  return {static_cast<float>(pos.x) / static_cast<float>(Chunk::pWidth),
      static_cast<float>(pos.y) / static_cast<float>(Chunk::pHeight)};
}

auto	Camera::sToWPos(const Vector2i &chunkId, const Vector2f &position) const -> worldPos
{
  return worldPos(static_cast<float>(chunkId.x) + position.x,
		  static_cast<float>(chunkId.y) + position.y);
}
