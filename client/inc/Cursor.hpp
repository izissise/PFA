#ifndef _CURSOR_H_
#define _CURSOR_H_

#include <SFML/Graphics.hpp>

class Cursor
{
public:
  Cursor(const sf::Text &text = sf::Text(), float blink = 700);
  virtual ~Cursor();

  void		setCursorPos(float x, float y);
  void		setCursorPos(const sf::Text &text);
  void		setText(const sf::Text &text);
  void		update();
  void		draw(sf::RenderWindow &window) const;
  std::size_t	getWidth() const;
  unsigned int	getSize() const;
  void		setColor(const sf::Color &color);

private:
  sf::Text	_cursor;
  sf::Clock	_time;
  float		_blinkTime;
  bool		_display;
};

#endif /* _CURSOR_H_ */
