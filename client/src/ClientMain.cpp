#include "ClientMain.hpp"

ClientMain::ClientMain()
  : _settings(),
    _parser(&_settings),
    _menu(_settings, _parser),
    _window(sf::VideoMode(std::stoi(_settings.getCvarList().getCvar("r_width")),
                          std::stoi(_settings.getCvarList().getCvar("r_height"))), "Name"),
  _showMenu(false),
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

	time.start();
	while (_window.isOpen())
   	{
   	 _menu.update(time.GetTargetFrameDuration(), _settings);
   	  time.endFrame();
   	}
}

void ClientMain::run()
{
  Controls	&ctrl = _settings.getControls();
  sf::Event	event;
  t_entry	entry;
  int		gameFps = std::stoi(_settings.getCvarList().getCvar("com_gameFps"));

  int gfps = 1000 / gameFps;
  TimeHandling time((std::chrono::milliseconds(gfps)));

  _window.setFramerateLimit(gameFps);
  _window.setVerticalSyncEnabled(true);
  time.start();
  //_window.setKeyRepeatEnabled(false);
  while (_window.isOpen())
    {
      _window.clear();
      while (_window.pollEvent(event))
        {
          if (event.type == sf::Event::Closed)
            {
              _window.close();
              break ;
            }
          if (!_menu.run(event, _window, _settings))
            {
	      entry.fill(event);
	      if (event.type == sf::Event::KeyPressed ||
		  event.type == sf::Event::MouseButtonPressed ||
		  event.type == sf::Event::MouseWheelMoved)
                {
		  //std::cout << "keypress" << std::endl;
		  if (event.type == sf::Event::KeyPressed ||
		      event.type == sf::Event::MouseButtonPressed)
		    ctrl.pressKey(entry);
                }
              else if (event.type == sf::Event::KeyReleased ||
		       event.type == sf::Event::MouseButtonReleased)
                ctrl.releaseKey(entry);
            }
        }
      time.endFrame();
      // draw stuff here
      _menu.draw(_window);
      _window.display();
    }
}
