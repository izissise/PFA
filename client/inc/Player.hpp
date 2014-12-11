#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <cmath>

#include "AMovable.hpp"
#include "Range2.hpp"
#include "Camera.hpp"
#include "TileCodex.hpp"


class Player : public AMovable
{
public:
  Player(Camera &camera);
  virtual ~Player();

  void		move(const Vector2f &dir);

  Range2i	&getLoadedRange();
  Range2i	&getVisibleRange();
  const Range2i	&getVisibleRange() const;
  void		calculateVisibleRange();

  void		setLoadedRange(const Range2i &range);
  void		setPlayerPosition(const Vector2i &chunkId,
				  const Vector2f &position);

private:
  Range2i	_loadedRange;
  Range2i	_visibleRange;
  Camera	&_camera;
};

#endif /* _PLAYER_H_ */
