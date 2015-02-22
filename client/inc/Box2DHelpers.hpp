#ifndef BOX2DHELPERS_H
#define BOX2DHELPERS_H

#include <memory>
#include <functional>

#include <Box2D/Box2D.h>

#include "MakeRessource.hpp"

class Box2DHelpers
{
  Box2DHelpers() = delete;
  ~Box2DHelpers() = delete;
public:
  static auto createBody(std::shared_ptr<b2World> const& world,
                         b2BodyDef const& bodyDef, b2Shape const& shape,
                         float density)
  {
    std::weak_ptr<b2World> weakWorld(world);

    auto constructor = [](std::shared_ptr<b2World> const& world, b2BodyDef const & bodyDef, b2Shape const & shape, float density)
    {
      b2Body* body = world->CreateBody(&bodyDef);
      body->CreateFixture(&shape, density);
      return body;
    };

    auto deconstructor = [weakWorld](b2Body * bud) {
      if (auto world = weakWorld.lock())
        world->DestroyBody(bud);
    };
    return make_resource(constructor, deconstructor, world, bodyDef, shape, density);
  };

};

#endif // BOX2DHELPERS_H
