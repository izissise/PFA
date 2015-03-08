#include "AMovable.hpp"

AMovable::AMovable()
{
}

void AMovable::update(std::chrono::milliseconds const & timeStep)
{
}

bool	AMovable::move(const Vector2f &dir)
{
  bool	side;
  bool	crossChunk = false;

  _pos.x += dir.x;
  _pos.y += dir.y;
  if (_pos.x >= 1 || _pos.x < 0)
    {
      side = (_pos.x >= 1);
      _pos.x += (side ? (-1.f) : (1.f));
      _chunkId.x += (side ? 1 : -1);
      crossChunk = true;
    }
  if (_pos.y >= 1 || _pos.y < 0)
    {
      side = (_pos.y >= 1);
      _pos.y += (side ? (-1.f) : (1.f));
      _chunkId.y += (side ? 1 : -1);
      crossChunk = true;
    }
  return crossChunk;
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
