#ifndef	LEVEL_H
# define LEVEL_H

# include <entityx/entityx.h>
# include <SFML/Graphics.hpp>
# include <SFML/Window.hpp>


class Level : public entityx::EntityX
{
public:
  Level(sf::RenderTarget &target, const sf::Font &font);
  ~Level() = default;
  Level(const Level &rhs) = delete;
  Level		&operator=(const Level &rhs) = delete;

  void		update(entityx::TimeDelta dt);
};

#endif /* LEVEL_H */
