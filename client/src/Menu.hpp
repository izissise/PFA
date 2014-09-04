#ifndef _MENU_H_
#define _MENU_H_

# include <SFML/Graphics.hpp>
# include <string>
# include "Settings.hpp"
# include "Console.hpp"

class Menu
{
public:
  Menu(Settings &set);
  virtual ~Menu();

  void updateThread();
  void	run(Settings &set, Console &con);

private:
  sf::RenderWindow		_window;
  sf::Texture			_background;
};

#endif /* _MENU_H_ */
