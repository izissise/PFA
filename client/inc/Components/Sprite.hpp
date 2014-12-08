#ifndef	SPRITE_H
# define SPRITE_H

# include <entityx/entityx.h>
# include <SFML/Graphics.hpp>

struct Sprite : public entityx::Component<Sprite>
{
  Sprite() {}
  Sprite(const sf::Texture &tex) : sprite(tex) {}
  Sprite(const sf::Texture &tex, const sf::IntRect &rectangle) : sprite(tex, rectangle) {}
  Sprite(const sf::Sprite &rhs) : sprite(rhs) {}

  sf::Sprite	sprite;
};

#endif /* SPRITE_H */
