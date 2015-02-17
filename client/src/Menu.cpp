#include <thread>
#include <chrono>
#include <ctime>
#include <thread>

#include "Menu.hpp"
#include "Exception.hpp"
#include "MainMenu.hpp"
#include "ServerMenu.hpp"
#include "GamePanel.hpp"
#include "OptionTabPanel.hpp"
#include "OptionGamePanel.hpp"
#include "OptionKeyPanel.hpp"

Menu::Menu(Settings &settings, Parser &parser) :
    _consoleActive(false),
    _console(settings, &parser),
    _panelPos(0)
{
  parser.loadConfigFile("../config.cfg");
  if (!_menuTexture.loadFromFile("../client/assets/menuTexture.png"))
    throw (Exception("Can't load Menu texture"));

  MainMenu		*mainMenu = new MainMenu(sf::FloatRect(0,0,SIZEX,SIZEY));
  ServerMenu		*serverMenu = new ServerMenu(sf::FloatRect(0,0,SIZEX,SIZEY));
  GamePanel		*gamePanel = new GamePanel(sf::FloatRect(0,0,SIZEX,SIZEY));
  OptionTabPanel	*optTabPanel = new OptionTabPanel(sf::FloatRect(380,50,1115,50));
  OptionGamePanel	*optGamePanel = new OptionGamePanel(sf::FloatRect(380,100,1130,730));
  OptionKeyPanel	*optKeyPanel = new OptionKeyPanel(sf::FloatRect(380,100,1130,730));

  _panels.push_back(mainMenu);
  _panels.push_back(serverMenu);
  _panels.push_back(gamePanel);

  mainMenu->addPanel({optTabPanel});
  optTabPanel->addPanel({optGamePanel, optKeyPanel});

  mainMenu->construct(_menuTexture, settings, {optTabPanel, serverMenu});
  serverMenu->construct(_menuTexture, settings, {mainMenu, gamePanel});
  gamePanel->construct(_menuTexture, settings, {mainMenu});
  optTabPanel->construct(_menuTexture, settings, {optGamePanel, optKeyPanel});
  optGamePanel->construct(_menuTexture, settings, {});
  optKeyPanel->construct(_menuTexture, settings, {});
}

Menu::~Menu()
{
}

void	Menu::update(const std::chrono::milliseconds timeStep, Settings &set)
{
  for (auto &panel : _panels)
    panel->update(timeStep, set);
}

bool	Menu::run(const sf::Event& ev, sf::RenderWindow &window, Settings &set)
{
  bool	handled = false;

  _consoleActive = set.getControls().getActionState(Action::ToggleConsole);
  if (_consoleActive)
    {
      _console.run(ev, set.getControls());
      handled = true;
    }
  else
    {
      for (auto &panel : _panels)
	{
	  if (!panel->isHidden())
	    if ((handled = (panel->event(ev, window, set) != 0)))
	      return handled;
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
