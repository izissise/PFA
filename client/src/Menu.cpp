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

  MainMenu	*mainMenu = new MainMenu(_menuTexture);
  _panels.push_back(mainMenu);
}

Menu::~Menu()
{
}

bool	Menu::run(const sf::Event& event, sf::RenderWindow &window, Settings &set)
{
  bool	handled = false;

  _consoleActive = set.getControls().getActionState(Action::ToggleConsole);
  if (_consoleActive)
    {
      _console.run(event);
      handled = true;
    }
  else
    _panels[_panelPos]->run(event, window, set);
  return handled;
}

void	Menu::draw(sf::RenderWindow& window)
{
  if (_consoleActive)
    _console.draw(window);
  else
    _panels[_panelPos]->draw(window);
}
