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

void		Menu::run(Settings &set)
{
  sf::Event	event;
  sf::Sprite	sprite(_background);
  Controls	ctrl = set.getControls();

  while (_window.isOpen())
    {
      while (_window.pollEvent(event))
	{
	  if (event.type == sf::Event::Closed)
	    _window.close();
	  else if (event.type == sf::Event::KeyPressed)
	    {
	      try {
		std::cout << "Action for key " << ctrl.getCodeFromKey(event.key.code) << " is: ";
	      }
	      catch (const Exception &e) {
		std::cout << "Action for key " << "Unknown" << " is: ";
	      }
	      try {
		std::cout << ctrl.getCodeFromAction(ctrl.getActionFromKey(event.key.code)) << std::endl;
	      }
	      catch (const std::out_of_range &oor) {
		std::cout << "Unknown" << std::endl;
	      }
	    }
	}
      _window.clear();
      _window.draw(sprite);
      _window.display();
    }
}
