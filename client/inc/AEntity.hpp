#ifndef AENTITY_HPP
#define AENTITY_HPP

#include <memory>

#include <Box2D/Box2D.h>

#include "AMovable.hpp"


class AEntity : public AMovable
{
public:
  AEntity(std::shared_ptr<b2World> const& world);
  virtual ~AEntity() {};

  virtual void update(std::chrono::milliseconds const & timeStep);

  virtual void draw(sf::RenderTarget& window, std::chrono::milliseconds const & timeStep) const;

protected:
	std::unique_ptr<b2Body, std::function<void(b2Body*)>> _body;
};

#endif // AENTITY_HPP
