#include <iostream>
#include "MainMenu.hpp"

MainMenu::MainMenu(sf::Texture * const texture, sf::Texture * const background) :
  APanelScreen(texture, background)
{
  if (!_font.loadFromFile("../client/assets/font.otf"))
    std::cerr << "Can't load font" << std::endl; // replace this by a throw about ressources
  _widgets.push_back(new Widget("Play", {50, 100}, new sf::Text("Play", _font, 30)));
}

MainMenu::~MainMenu()
{
}

int	MainMenu::run(sf::Event event)
{
}
