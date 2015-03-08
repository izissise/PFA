#ifndef _AMOVABLE_H_
#define _AMOVABLE_H_

#include <chrono>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Vector2.hpp"
#include "Config.h"

class AMovable
{
public:
  AMovable();
  virtual ~AMovable() {};

  virtual void update(std::chrono::milliseconds const & timeStep) = 0;

  virtual bool		move(const Vector2f &dir);
  const Vector2f	&getPosition() const;
  const Vector2i	&getChunkId() const;

  void			setPosition(const Vector2f &position);
  void			setChunkId(const Vector2i chunkId);

protected:
  Vector2f	_pos;
  Vector2i	_chunkId;
};

#endif /* _AMOVABLE_H_ */
