#ifndef _MENU_H_
#define _MENU_H_

# include <string>
# include <chrono>

# include <SFML/Graphics.hpp>

# include "Parser.hpp"
# include "Console.hpp"
# include "APanelScreen.hpp"

class Menu
{
public:
  Menu(Settings &set, Parser &parser);
  virtual ~Menu();

  void		update(const std::chrono::milliseconds timeStep, Settings &set);
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
