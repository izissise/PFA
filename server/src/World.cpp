#include "World.hpp"
#include "Perlin.h"

World::World(ServerSettings &cvars) :
  _cvars(cvars)
{
  noise::setSeed(std::stoi(cvars.getCvar("s_seed")));
  noise::initPerm();
}

