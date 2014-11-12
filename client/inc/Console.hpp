#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <map>

#include "UserInput.hpp"
#include "Cursor.hpp"
#include "Parser.hpp"
#include "Historical.hpp"

#define FONTSIZE 20
#define LINESPACING 4

class Console
{
public:
  Console(Settings &set, Parser *parser);
  virtual ~Console();

  void		run(const sf::Event& event, Controls &ctrl);
  void		draw(sf::RenderWindow &window);

private:
  void		getInput();

private:
  UserInput		_input;
  Cursor		_cursor;
  Historical		_historical;
  sf::RectangleShape	_rectangle;
  sf::Font		_font;
  sf::Text		_text;
  Parser		*_parser;
};

#endif /* _CONSOLE_H_ */
