#include "APanelScreen.hpp"

APanelScreen::APanelScreen(const sf::Texture &texture)
{
}

APanelScreen::~APanelScreen()
{
}

void		APanelScreen::draw(sf::RenderWindow &window)
{
  for (auto &widget : _widgets)
    widget->draw(window);
}

void		APanelScreen::saveTexture(sf::Texture * const texture)
{
  _textures.push_back(texture);
}

const sf::Texture	*APanelScreen::addSpriteForWidget(AWidget * const widget,
							  const sf::Color &color,
							  const sf::Vector2u &size)
{
  sf::Image	image;
  sf::Texture	*texture = new sf::Texture;
  sf::Sprite	sprite;

  image.create(size.x, size.y, color);
  texture->loadFromImage(image);
  sprite.setTexture(*texture);
  saveTexture(texture);
  widget->addSprite(sprite);
  return (_textures.back());
}
