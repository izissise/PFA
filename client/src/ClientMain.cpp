#include "ClientMain.hpp"

ClientMain::ClientMain()
  : _window(sf::VideoMode(std::stoi(_settings.getCvarList().getCvar("r_width")),
                          std::stoi(_settings.getCvarList().getCvar("r_height"))), "Name"),
  _menu(_settings),
  _updateThread(std::bind(&ClientMain::update, this))
{
  _window.setVerticalSyncEnabled(true);
}

ClientMain::~ClientMain()
{
  _updateThread.join();
}

void ClientMain::update()
{
  int gfps = 1000 / std::stoi(_settings.getCvarList().getCvar("com_gameFps"));
  TimeHandling time((std::chrono::milliseconds(gfps)));

  // the updating loop
  time.start();
  while (_window.isOpen())
    {

//update object here
      time.endFrame();
    }
}

void ClientMain::run()
{
  Controls &ctrl = _settings.getControls();
  sf::Event event;

  while (_window.isOpen())
    {
      while (_window.pollEvent(event))
        {
          if (event.type == sf::Event::Closed)
            {
              _window.close();
              break ;
            }
          if (!_menu.run(event, ctrl))
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

      //draw stuff here
      _menu.draw(_window);

      _window.display();
    }

}
