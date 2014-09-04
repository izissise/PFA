#include "APanelScreen.hpp"

APanelScreen::APanelScreen(sf::Texture * const texture) : _texture(texture)
{
}

APanelScreen::~APanelScreen()
{
}

void	APanelScreen::draw(sf::RenderWindow &window)
{
  for (auto &elem : _sprites)
    window.draw(elem.second, _texture);
  window.draw(_text);
}
