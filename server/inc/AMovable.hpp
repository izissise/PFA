#ifndef _AMOVABLE_H_
#define _AMOVABLE_H_

#include "Vector2.hpp"

class AMovable
{
public:
  enum Direction
    {
      Up,
      Right,
      Down,
      Left
    };

public:
  AMovable();
  virtual ~AMovable();

  void			move(Direction dir);
  const Vector2f	&getPosition() const;
  const Vector2i	&getChunkId() const;
  Direction		getDirection() const;

private:
  Vector2f	_pos;
  Vector2i	_chunkId;
  Direction	_dir;
};

#endif /* _AMOVABLE_H_ */
