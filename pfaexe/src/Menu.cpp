#include "Menu.hpp"

#include <iostream>

Menu::Menu(Settings &set) :
  _window(sf::VideoMode(std::stoi(set.getCvarList().getCvar("r_width")),
			std::stoi(set.getCvarList().getCvar("r_height"))), "Name")
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
