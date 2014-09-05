#include <iostream>
#include "MainMenu.hpp"

MainMenu::MainMenu(sf::Texture * const texture, sf::Texture * const background) :
  APanelScreen(texture, background)
{
  if (!_font.loadFromFile("../client/assets/font.otf"))
    std::cerr << "Can't load font" << std::endl; // replace this by a throw about ressources
  Widget	*playWidget = new Widget("play", {50, 100}, new sf::Text("PLAY", _font, 30));
  Widget	*optWidget = new Widget("opt", {50, 200}, new sf::Text("OPTION", _font, 30));
  Widget	*quitWidget = new Widget("quit", {50, 300}, new sf::Text("QUIT", _font, 30));
  sf::Sprite	sprite(*texture, sf::IntRect(0, 0, 100, 50));

  playWidget->alignText({50,100}, {100, 50}, 50, 50);
  playWidget->addSprite(sprite);
  optWidget->alignText({50,200}, {100, 50}, 50, 50);
  optWidget->addSprite(sprite);
  quitWidget->alignText({50,300}, {100, 50}, 50, 50);
  quitWidget->addSprite(sprite);
  _widgets.push_back(playWidget);
  _widgets.push_back(optWidget);
  _widgets.push_back(quitWidget);
}

MainMenu::~MainMenu()
{
}

int	MainMenu::run(sf::Event event)
{
  for (auto &widget : _widgets)
    widget->update(event);
}
