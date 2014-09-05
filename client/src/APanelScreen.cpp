#include "APanelScreen.hpp"

APanelScreen::APanelScreen(sf::Texture * const texture, sf::Texture * const background) :
  _texture(texture), _background(background)
{
}

APanelScreen::~APanelScreen()
{
}

void		APanelScreen::draw(sf::RenderWindow &window)
{
  if (_background)
    {
      sf::Sprite	background(*_background);
      window.draw(background);
    }
  for (auto &widget : _widgets)
    widget->draw(window);
}
