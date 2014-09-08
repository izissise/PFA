#include <thread>
#include <chrono>
#include <ctime>
#include <thread>

#include "Menu.hpp"
#include "Exception.hpp"
#include "MainMenu.hpp"

Menu::Menu(Settings& settings)
  : _console(&settings),
    _consoleActive(false),
    _panelPos(0)
{
  if (!_menuTexture.loadFromFile("../client/assets/menuTexture.png"))
    throw (Exception("Can't load Menu texture"));

  MainMenu	*mainMenu = new MainMenu(_menuTexture, settings);

  _panels.push_back(mainMenu);
}

Menu::~Menu()
{
}

bool	Menu::run(const sf::Event& event, sf::RenderWindow &window, Settings &set)
{
  bool	handled = false;
  int	retVal;

  _consoleActive = set.getControls().getActionState(Action::ToggleConsole);
  if (_consoleActive)
    {
      _console.run(event);
      handled = true;
    }
  else
    {
      std::string	ret;

      if ((retVal = _panels[_panelPos]->run(event, window, set)) != 0)
	{
	  ret = std::to_string(retVal);
	  //	  std::cout << ret << std::endl;
	  if (ret.at(0) == '2')
	    _panelPos = ret.at(2) - '0';
	}
    }
  return handled;
}

void	Menu::draw(sf::RenderWindow& window)
{
  if (_consoleActive)
    _console.draw(window);
  else
    _panels[_panelPos]->draw(window);
}
