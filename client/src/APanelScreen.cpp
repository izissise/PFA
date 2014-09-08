#include "APanelScreen.hpp"

APanelScreen::APanelScreen(const sf::Texture &texture, Settings &set)
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

sf::Vector2f	APanelScreen::toPixel(const sf::Vector2f &perCent,
				      const sf::Vector2f &size) const
{
  return sf::Vector2f(perCent.x * size.x, perCent.y * size.y);
}

sf::FloatRect	APanelScreen::toPixel(const sf::FloatRect &perCent,
				      const sf::Vector2f &size) const
{
  return sf::FloatRect(perCent.left * size.x, perCent.top * size.y,
		       perCent.width * size.x, perCent.height * size.y);
}

void		APanelScreen::resizeWidgets(const sf::Vector2f &size)
{
  for (auto &elem : _widgets)
    elem->resize(size);
}

void		APanelScreen::saveTexture(sf::Texture * const texture)
{
  _textures.push_back(texture);
}

const sf::Texture	*APanelScreen::addSpriteForWidget(AWidget * const widget,
							  const sf::Color &color,
							  const sf::Vector2f &size)
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
