#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <cmath>
#include <memory>

#include "AEntity.hpp"
#include "Range2.hpp"
#include "Camera.hpp"
#include "ATool.hpp"
#include "AItem.hpp"

class Player : public AEntity
{
private:
  static const unsigned int defaultDamages = 1;
public:
  Player(std::shared_ptr<b2World> const& world, Camera &camera);
  virtual ~Player() {};

  void update(std::chrono::milliseconds const & timeStep) override;
 // void draw(sf::RenderTarget& window, std::chrono::milliseconds const & timeStep) const override;
  bool		move(const Vector2f &dir) override;

  Range2i	&getLoadedRange();
  Range2i	&getVisibleRange();
  const Range2i	&getVisibleRange() const;
  void		calculateVisibleRange();

  void		setLoadedRange(const Range2i &range);
  void		setPlayerPosition(const Vector2i &chunkId,
				  const Vector2f &position,
				  float rWidth, float rHeight);
  void		hit(AItem &item);

private:
  Range2i	_loadedRange;
  Range2i	_visibleRange;
  Camera	&_camera;
  std::unique_ptr<ATool>	_tool;
};

#endif /* _PLAYER_H_ */
