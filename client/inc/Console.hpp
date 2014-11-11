#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <SFML/Graphics.hpp>
#include <deque>
#include <map>
#include "UserInput.hpp"
#include "Cursor.hpp"
#include "Parser.hpp"

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
  Console(Settings &set, Parser *parser);
  virtual ~Console();

  void		run(const sf::Event& event, Controls &ctrl);
  void		draw(sf::RenderWindow &window);

private:
  UserInput		_input;
  Cursor		_cursor;
  t_history		_history;
  sf::RectangleShape	_rectangle;
  sf::Font		_font;
  sf::Text		_text;
  Parser		*_parser;
};

#endif /* _CONSOLE_H_ */
