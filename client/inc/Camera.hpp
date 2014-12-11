#ifndef _CAMERA_H_
#define _CAMERA_H_

# include "Vector2.hpp"

class Camera
{
private:
  typedef Vector2f	worldPos;
  typedef Vector2i	screenPos;
  typedef Vector2i	chunkId;

public:
  Camera(void) = default;
  ~Camera(void) = default;
  Camera(const Camera& other) = default;
  Camera&	operator=(const Camera& other) = default;

  void	translate(const worldPos& v);
  void	move(const worldPos& pos);

  void	scale(const worldPos& s);
  void	resize(const worldPos& s);

  float	left(void) const	{ return _botLeft.x; }
  float	right(void) const	{ return _botLeft.x + _size.w; }
  float	top(void) const		{ return _botLeft.y + _size.h; }
  float	bottom(void) const	{ return _botLeft.y; }
  float	width(void) const	{ return _size.w; }
  float	height(void) const	{ return _size.h; }
  worldPos	center(void) const	{ return _center; }

private:
  worldPos	_botLeft;
  worldPos	_size;
  worldPos	_center;
};

#endif /* _CAMERA_H_ */
