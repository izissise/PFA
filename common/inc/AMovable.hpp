#ifndef _AMOVABLE_H_
#define _AMOVABLE_H_

#include "Vector2.hpp"

class AMovable
{
public:
  AMovable();
  virtual ~AMovable();

  virtual void		move(const Vector2f &dir);
  const Vector2f	&getPosition() const;
  const Vector2i	&getChunkId() const;

  void			setPosition(const Vector2f &position);
  void			setChunkId(const Vector2i chunkId);

private:
  Vector2f	_pos;
  Vector2i	_chunkId;
};

#endif /* _AMOVABLE_H_ */
