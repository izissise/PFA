#ifndef	RENDERSYSTEM_H
# define RENDERSYSTEM_H

# include <entityx/entityx.h>
# include <SFML/Graphics.hpp>
# include <SFML/Window.hpp>

namespace Systems
{
  class Render : public entityx::System<Render>
  {
  public:
    Render(sf::RenderTarget &target, const sf::Font &font);
    virtual ~Render() = default;
    Render(const Render &) = delete;
    Render	&operator=(const Render &) = delete;

    void	update(entityx::EntityManager &es,
		       entityx::EventManager &events,
		       entityx::TimeDelta dt);

  private:
    float		_last_update = 0.0f;
    sf::RenderTarget	&_target;
    sf::Text		_text;
  };
};

#endif /* RENDERSYSTEM_H */
