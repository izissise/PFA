#include <thread>
#include <chrono>
#include <ctime>
#include <ratio>
#include "Menu.hpp"
#include "Exception.hpp"
#include "MainMenu.hpp"

Menu::Menu(Settings &set) :
  _window(sf::VideoMode(std::stoi(set.getCvarList().getCvar("r_width")),
			std::stoi(set.getCvarList().getCvar("r_height"))), "Name")
{
  _panelPos = 0;
}

Menu::~Menu()
{
}

void		Menu::run(Settings &set, Console &con)
{
  sf::Event	event;
  Controls	&ctrl = set.getControls();
  double	fps = 1000.0 / std::stod(set.getCvarList().getCvar("com_fps"));
  std::chrono::duration<double, std::milli>	time;
  std::chrono::steady_clock::time_point		begin;
  bool						console = false;

  // Loading Textures and initializing panels

  sf::Texture	menuTexture;

  if (!menuTexture.loadFromFile("../client/assets/menuTexture.png"))
    throw (Exception("Can't load background file"));
  MainMenu	*mainPanel = new MainMenu(&menuTexture);

  _panels.push_back(mainPanel);
  while (_window.isOpen())
    {
      begin = std::chrono::steady_clock::now();
      _window.clear();
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
	      _panels[_panelPos]->run(event, _window);
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
      _panels[_panelPos]->draw(_window);
      console = ctrl.getActionState(Action::ToggleConsole);
      if (console)
	con.draw(_window);
      time = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>
	(std::chrono::steady_clock::now() - begin);
      if (time.count() < fps)
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(fps - time.count())));
      _window.display();
    }
}
