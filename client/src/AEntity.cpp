#include "AEntity.hpp"

#include <memory>

#include "Box2DHelpers.hpp"

AEntity::AEntity(std::shared_ptr<b2World> const& world)
  : _body(nullptr, nullptr)
{
  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set(0.0f, -10.0f);

  b2PolygonShape groundBox;
  groundBox.SetAsBox(50.0f, 10.0f);

  _body = Box2DHelpers::createBody(world, groundBodyDef, groundBox, 0.0f);
}


void AEntity::update(std::chrono::milliseconds const & timeStep)
{

}

void AEntity::draw(sf::RenderTarget& window, std::chrono::milliseconds const & timeStep) const
{
}
