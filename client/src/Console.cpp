#include "Console.hpp"

Console::Console(Settings * const set) : _set(set)
{
}

Console::~Console()
{
}


void		Console::run(sf::Event event)
{
  Controls	&ctrl = _set->getControls();

  if (event.type == sf::Event::KeyPressed)
    ctrl.pressKey(event.key.code);
  if (event.type == sf::Event::KeyReleased)
    ctrl.releaseKey(event.key.code);
}
