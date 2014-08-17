#include "Menu.hpp"

Menu::Menu() : _window(sf::VideoMode(1600, 900), "Name")
{
}

Menu::~Menu()
{
}

void		Menu::run()
{
  sf::Event	event;

  while (_window.isOpen())
    {
      while (_window.pollEvent(event))
	{
	  if (event.type == sf::Event::Closed)
	    _window.close();
	}
    }
}
