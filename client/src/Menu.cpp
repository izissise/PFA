#include <thread>
#include <chrono>
#include <ctime>
#include <ratio>
#include "Menu.hpp"
#include "Exception.hpp"
#include "UserInput.hpp"

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
  UserInput    	input;
  //  sf::Text	text;
  Controls	&ctrl = set.getControls();
  double	fps = 1000.0 / std::stod(set.getCvarList().getCvar("com_fps"));
  std::chrono::duration<double, std::milli>	time;
  std::chrono::steady_clock::time_point		begin;

  //  _window.setKeyRepeatEnabled(false);
  // text.setString(strtext);
  while (_window.isOpen())
    {
      begin = std::chrono::steady_clock::now();
      while (_window.pollEvent(event))
        {
	  if (event.type == sf::Event::Closed)
	    {
	      _window.close();
	      break ;
	    }
	  if (ctrl.getActionState(Action::ToggleConsole))
	    con.run(event);
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
	      if (input.getInput(event))
		{
		  std::cout << "Input: " << input.getString().toAnsiString() << std::endl;
		  input.clear();
		}
	    }
	}
      time = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>
	(std::chrono::steady_clock::now() - begin);
      if (time.count() < fps)
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(fps - time.count())));
      _window.clear();
      _window.draw(sprite);
      //      _window.draw(text);
      _window.display();
    }
}
