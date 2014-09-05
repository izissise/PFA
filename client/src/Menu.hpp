#ifndef _MENU_H_
#define _MENU_H_

# include <SFML/Graphics.hpp>
# include <string>
# include "Settings.hpp"
# include "Console.hpp"
# include "APanelScreen.hpp"

class Menu
{
public:
  Menu(Settings &set);
  virtual ~Menu();

  void	run(Settings &set, Console &con);

private:
  sf::RenderWindow		_window;
  std::vector<APanelScreen *>	_panels;
  int				_panelPos;
};

#endif /* _MENU_H_ */
