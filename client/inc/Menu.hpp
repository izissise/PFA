#ifndef _MENU_H_
# define _MENU_H_

# include <SFML/Graphics.hpp>
# include <string>
# include "Settings.hpp"
# include "Console.hpp"

class Menu
{
public:
  Menu();
  virtual ~Menu();

  void run();
  void draw(sf::RenderWindow& window) const;

private:
  sf::Texture			_background;
};

#endif /* _MENU_H_ */
