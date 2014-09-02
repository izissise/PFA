#include "Console.hpp"

Console::Console(Settings * const set) : _set(set)
{
}

Console::~Console()
{
}


void		Console::run(sf::RenderWindow &window)
{
  Controls	ctrl = _set->getControls();
  sf::Event	event;
  bool		runConsole = true;

  while (runConsole)
    {
      while (window.pollEvent(event))
        {
	  if (event.type == sf::Event::Closed || ctrl.getActionState(Action::ToggleConsole))
	    runConsole = false;
	}
    }
}
