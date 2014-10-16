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

  bool		run(const sf::Event &event, sf::RenderWindow &window, Settings &set);
  void		draw(sf::RenderWindow &window);

private:
  bool				_consoleActive;
  Console			_console;
  sf::Texture			_menuTexture;
  std::vector<APanelScreen *>	_panels;
  int				_panelPos;
};

#endif /* _MENU_H_ */
