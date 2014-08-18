#ifndef _MENU_H_
#define _MENU_H_

# include <SFML/Window.hpp>
# include <string>
# include "Settings.hpp"

class Menu
{
public:
  Menu(Settings &set);
  virtual ~Menu();

  void	run();

private:
  sf::Window		_window;
};

#endif /* _MENU_H_ */
