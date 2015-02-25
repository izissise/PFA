#ifndef _CURSOR_H_
#define _CURSOR_H_

#include <SFML/Graphics.hpp>

#include "BlinkText.hpp"

class Cursor : public BlinkText
{
public:
  Cursor(const sf::Text &text = sf::Text(), unsigned int blinkTime = 700);
  virtual ~Cursor();

  void		setCursorPos(float x, float y);
  void		setCursorPos(const sf::Text &text);
  void		setText(const sf::Text &text);
  void		update();
  void		draw(sf::RenderTarget &window) const;
  std::size_t	getWidth() const;
  unsigned int	getSize() const;
  void		setColor(const sf::Color &color);
  void		scale(float ratioX, float ratioY);

private:
  sf::Text	_cursor;
};

#endif /* _CURSOR_H_ */
