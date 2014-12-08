#include "Systems/Render.hpp"
#include "Level.hpp"

Level::Level(sf::RenderTarget &target, const sf::Font &font)
{
  systems.add<Systems::Render>(target, font);
  systems.configure();
}

void	Level::update(entityx::TimeDelta dt)
{
  systems.update<Systems::Render>(dt);
}
