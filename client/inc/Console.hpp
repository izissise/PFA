#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <SFML/Graphics.hpp>
#include <deque>
#include "Settings.hpp"
#include "UserInput.hpp"

#define FONTSIZE 20
#define LINESPACING 4

typedef struct			s_history
{
  std::deque<sf::String>	content;
  unsigned int			maxSize;
  unsigned int			pos;

  s_history(unsigned int s = 100) : maxSize(s), pos(0)
  {
  }
}				t_history;

class Console
{
public:
  Console(Settings * const set);
  virtual ~Console();

  void		run(sf::RenderWindow &window, sf::Event event);
  void		draw(sf::RenderWindow &window);

private:
  Settings		*_set;
  UserInput		_input;
  t_history		_history;
  sf::RectangleShape	_rectangle;
  sf::Font		_font;
  sf::Text		_text;
};

#endif /* _CONSOLE_H_ */
