#include <sstream>

#include <entityx/Entity.h>

#include "Unused.hpp"

#include "Components/Position.hpp"
#include "Components/Sprite.hpp"
#include "Systems/Render.hpp"

namespace Systems
{
  Render::Render(sf::RenderTarget &target, const sf::Font &font) :
    _target(target)
  {
    _text.setFont(font);
    _text.setPosition({2, 2});
    _text.setCharacterSize(18);
    _text.setColor(sf::Color::White);
  }

  void		Render::update(entityx::EntityManager &es,
			       UNUSED entityx::EventManager &events,
			       entityx::TimeDelta dt)
  {
    Position::Handle	position;
    Sprite::Handle	sprite;

    for (entityx::Entity entity : es.entities_with_components(position, sprite))
      {
	sprite->sprite.setPosition(position->x, position->y);
	_target.draw(sprite->sprite);
      }
    _last_update += dt;
    if (_last_update >= 0.5f)
      {
	std::ostringstream	out;

	out << es.size() << " entities (" << static_cast<int>(1.0 / dt) << " fps)";
	_text.setString(out.str());
	_last_update = 0.0f;
      }
    _target.draw(_text);
  }
};
