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
  const Vector2i	&getPosition() const;
  Direction		getDirection() const;

private:
  Vector2i	_pos;
  Direction	_dir;
};

#endif /* _AMOVABLE_H_ */
