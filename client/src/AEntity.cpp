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

  _pos = position;
}

void AEntity::draw(Camera const& cam, sf::RenderTarget& window, std::chrono::milliseconds const & timeStep) const
{
  sf::CircleShape circle(32);
  Vector2i chunkSize;
  chunkSize.x = -Chunk::width;
  chunkSize.y = Chunk::height;

 Vector2i pos = cam.physWorldToSPos(_pos);
 pos += cam.center() * chunkSize * static_cast<int>(TileCodex::tileSize);
//   std::cout << "Pos at drawing: " << pos << std::endl;
  circle.setPosition(pos.x, pos.y);

// change the radius to 40
  circle.setRadius(40);

// change the number of sides (points) to 100
  circle.setPointCount(100);
  window.draw(circle);
}
