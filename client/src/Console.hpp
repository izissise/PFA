#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <SFML/Graphics.hpp>
#include "Settings.hpp"
#include "UserInput.hpp"

class Console
{
public:
  Console(Settings * const set);
  virtual ~Console();

  void		run(sf::RenderWindow &window, sf::Event event);
  void		draw(sf::RenderWindow &window) const;

private:
  Settings		*_set;
  UserInput		_input;
  sf::RectangleShape	_rectangle;
  sf::Font		_font;
  sf::Text		_text;
};

#endif /* _CONSOLE_H_ */
