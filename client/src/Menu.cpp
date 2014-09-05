#include <thread>
#include <chrono>
#include <ctime>
#include <thread>

#include "Menu.hpp"
#include "Exception.hpp"

Menu::Menu()
{
  if (!_background.loadFromFile("../client/assets/background.jpg"))
    throw (Exception("Cant load Menu's background file"));
}

Menu::~Menu()
{
}

void Menu::run()
{
}

void Menu::draw(sf::RenderWindow& window) const
{
  sf::Sprite	sprite(_background);

  window.draw(sprite);
}
