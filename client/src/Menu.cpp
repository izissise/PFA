#include <thread>
#include <chrono>
#include <ctime>
#include <thread>

#include "Menu.hpp"
#include "Exception.hpp"
#include "MainMenu.hpp"
#include "OptionTabPanel.hpp"
#include "OptionGamePanel.hpp"
#include "OptionKeyPanel.hpp"

Menu::Menu(Settings& settings) :
    _consoleActive(false),
    _console(&settings),
    _panelPos(0)
{
  if (!_menuTexture.loadFromFile("../client/assets/menuTexture.png"))
    throw (Exception("Can't load Menu texture"));

  MainMenu		*mainMenu = new MainMenu(sf::FloatRect(0,0,1600,900));
  OptionTabPanel	*optTabPanel = new OptionTabPanel(sf::FloatRect(0,0,1600,900));
  OptionGamePanel	*optGamePanel = new OptionGamePanel(sf::FloatRect(380,120,1123,730));
  OptionKeyPanel	*optKeyPanel = new OptionKeyPanel(sf::FloatRect(380,120,1123,730));

  _panels.push_back(mainMenu);
  _panels.push_back(optTabPanel);
  optTabPanel->addPanels({optGamePanel, optKeyPanel});

  mainMenu->construct(_menuTexture, settings, {optTabPanel});
  optTabPanel->construct(_menuTexture, settings, {optGamePanel, optKeyPanel});
  optGamePanel->construct(_menuTexture, settings, {});
  optKeyPanel->construct(_menuTexture, settings, {});
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
    {
      for (auto &panel : _panels)
	{
	  if (!panel->isHidden())
	    panel->run(event, window, set);
	}
    }
  return handled;
}

void	Menu::draw(sf::RenderWindow& window)
{
  for (auto &panel : _panels)
    {
      if (!panel->isHidden())
	panel->draw(window, true);
    }
  if (_consoleActive)
    _console.draw(window);
}
