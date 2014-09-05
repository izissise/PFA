#ifndef _MENU_H_
# define _MENU_H_

# include <SFML/Graphics.hpp>
# include <string>
# include "Settings.hpp"
# include "Console.hpp"

class Menu
{
public:
  Menu(Settings& settings);
  virtual ~Menu();

  bool run(const sf::Event& event, const Controls& ctrl);
  void draw(sf::RenderWindow& window);

private:
  Console _console;
  bool _consoleActive;
  sf::Texture _background;
};

#endif /* _MENU_H_ */
