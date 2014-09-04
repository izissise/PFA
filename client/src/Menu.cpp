#include <thread>
#include <chrono>
#include <ctime>
#include <thread>

#include "Menu.hpp"
#include "Exception.hpp"
#include "TimeHandling.hpp"

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

void Menu::updateThread()
{
// TimeHandling time(std::chrono::milliseconds(1000 / std::stoi(set.getCvarList().getCvar("com_gameFps"))));



  // the updating loop
// time.start();
  while (_window.isOpen())
    {


      std::this_thread::sleep_for(std::chrono::milliseconds(8));
      //time.endFrame();
    }
}


void		Menu::run(Settings &set, Console &con)
{
  sf::Event	event;
  Controls	&ctrl = set.getControls();
  sf::Sprite	sprite(_background);

  std::thread upThread(std::bind(&Menu::updateThread, this));

  _window.setVerticalSyncEnabled(true);
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
        }

      _window.draw(sprite);

      if (console)
        con.draw(_window);
      _window.display();
    }

  upThread.join();
}
