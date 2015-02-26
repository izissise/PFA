#include "AEntity.hpp"

#include <iostream>
#include <memory>

#include "Unused.hpp"
#include "Box2DHelpers.hpp"

AEntity::AEntity(std::shared_ptr<b2World> const& world)
  : _body(nullptr, nullptr)
{
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(0.0f, 4.0f);

  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(1.0f, 1.0f);

  _body = Box2DHelpers::createBody(world, bodyDef, dynamicBox, 1.0f, [](b2FixtureDef& fix)
  {
    fix.friction = 0.3f;
    fix.restitution = 0.8f;
  });

}


void AEntity::update(UNUSED std::chrono::milliseconds const & timeStep)
{
  Vector2f position = _body->GetPosition();

  _pos = position - Vector2f(0.0, -500.0f);

}

void AEntity::draw(sf::RenderTarget& window, std::chrono::milliseconds const & timeStep) const
{
  sf::Vector2u winSize = window.getSize();
  sf::CircleShape circle(32);

 // std::cout << "Pos at drawing: {" << _pos.x << ", " << _pos.y << "}" << std::endl;
  circle.setPosition(_pos.x, winSize.y - _pos.y);

// change the radius to 40
  circle.setRadius(40);

// change the number of sides (points) to 100
  circle.setPointCount(100);
  window.draw(circle);
}
