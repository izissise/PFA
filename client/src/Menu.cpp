#include <thread>
#include <chrono>
#include <ctime>
#include <thread>

#include "Menu.hpp"
#include "Exception.hpp"

Menu::Menu(Settings& settings)
  : _console(&settings),
    _consoleActive(false)
{
  if (!_background.loadFromFile("../client/assets/background.jpg"))
    throw (Exception("Cant load Menu's background file"));
}

Menu::~Menu()
{
}

bool Menu::run(const sf::Event& event, const Controls& ctrl)
{
  bool handled = false;

  _consoleActive = ctrl.getActionState(Action::ToggleConsole);
  if (_consoleActive)
    {
      _console.run(event);
      handled = true;
    }
  return handled;
}

void Menu::draw(sf::RenderWindow& window)
{
  sf::Sprite	sprite(_background);

  window.draw(sprite);

  if (_consoleActive)
    _console.draw(window);
}
