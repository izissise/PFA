#include "AMovable.hpp"

AMovable::AMovable()
{
}

AMovable::~AMovable()
{
}

void	AMovable::move(const Vector2f &dir)
{
  bool	side;

  _pos.x += dir.x;
  _pos.y += dir.y;
  if (_pos.x >= CHUNKWIDTH || _pos.x < 0)
    {
      side = (_pos.x >= CHUNKWIDTH);
      _pos.x += (side ? (-CHUNKWIDTH) : (CHUNKWIDTH));
      _chunkId.x += (side ? 1 : -1);
    }
  if (_pos.y >= CHUNKHEIGHT || _pos.y < 0)
    {
      side = (_pos.y >= CHUNKHEIGHT);
      _pos.y += (side ? (-CHUNKHEIGHT) : (CHUNKHEIGHT));
      _chunkId.y += (side ? 1 : -1);
    }
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
