#include "Menu.hpp"
#include "Exception.hpp"

Menu::Menu(Settings &set) :
  _window(sf::VideoMode(std::stoi(set.getCvarList().getCvar("r_width")),
			std::stoi(set.getCvarList().getCvar("r_height"))), "Name")
{
  if (!_background.loadFromFile("../pfaexe/assets/background.jpg"))
    throw (Exception("Cant load background file"));
}

Menu::~Menu()
{
}

void		Menu::run()
{
  sf::Event	event;
  sf::Sprite	sprite(_background);

  while (_window.isOpen())
    {
      while (_window.pollEvent(event))
	{
	  if (event.type == sf::Event::Closed)
	    _window.close();
	}
      _window.clear();
      _window.draw(sprite);
      _window.display();
    }
}
