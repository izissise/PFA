#ifndef _CAMERA_H_
#define _CAMERA_H_

# include "Vector2.hpp"
# include "Chunk.hpp"

class Camera
{
private:
  typedef Vector2f	worldPos;
  typedef Vector2i	screenPos;
  typedef Vector2i	chunkId;

public:
  Camera() = default;
  ~Camera() = default;
  Camera(const Camera& other) = default;
  Camera&	operator=(const Camera& other) = default;

  void	translate(const worldPos& v);
  void	move(const worldPos& pos);

  void	scale(const worldPos& s);
  void	resize(const worldPos& s);

  float	left() const	{ return _botLeft.x; }
  float	right() const	{ return _botLeft.x + _size.w; }
  float	top() const		{ return _botLeft.y + _size.h; }
  float	bottom() const	{ return _botLeft.y; }
  float	width() const	{ return _size.w; }
  float	height() const	{ return _size.h; }
  worldPos	center() const	{ return _center; }

  worldPos	sToWPos(const screenPos &pos) const;
  worldPos	sToWPos(const Vector2i &chunkId, const Vector2f &position) const;
  screenPos	wToSPos(const worldPos &pos) const;

private:
  worldPos	_botLeft;
  worldPos	_size;
  worldPos	_center;
};

#endif /* _CAMERA_H_ */
