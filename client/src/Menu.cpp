#include <thread>
#include <chrono>
#include <ctime>
#include <ratio>

#include "Menu.hpp"
#include "Exception.hpp"
#include "TimeHandling.hpp"

/* <TEST SECTION> */
#include "World.hpp"
/* </TEST SECTION> */

Menu::Menu(Settings &set) :
  _window(sf::VideoMode(std::stoi(set.getCvarList().getCvar("r_width")),
                        std::stoi(set.getCvarList().getCvar("r_height"))), "Name")
{
  if (!_background.loadFromFile("../client/assets/background.jpg"))
    throw (Exception("Cant load background file"));
}

Menu::~Menu()
{
}

void		Menu::run(Settings &set, Console &con)
{
  sf::Event	event;
  sf::Sprite	sprite(_background);
  Controls	&ctrl = set.getControls();
  TimeHandling	time(std::chrono::milliseconds(1000 / std::stoi(set.getCvarList().getCvar("com_gameFps"))));
  World		world(set);

  time.start();
  while (_window.isOpen())
    {
      bool console = ctrl.getActionState(Action::ToggleConsole);
      while (_window.pollEvent(event))
        {
          if (event.type == sf::Event::Closed)
            {
              _window.close();
              break ;
            }
          if (console)
            con.run(_window, event);
          else
            {
              if (event.type == sf::Event::KeyPressed)
                {
                  std::cout << "keypress" << std::endl;
                  ctrl.pressKey(event.key.code);
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
              else if (event.type == sf::Event::KeyReleased)
                ctrl.releaseKey(event.key.code);
            }
          time.endFrame();
        }

      _window.clear();
      _window.draw(sprite);
      world.draw(_window);
      if (console)
        con.draw(_window);
      _window.display();
    }
}
