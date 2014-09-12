#ifndef _TEXTWIDGET_H_
#define _TEXTWIDGET_H_

#include "AWidget.hpp"
#include "UserInput.hpp"
#include "Cursor.hpp"

class TextWidget : public AWidget, public UserInput
{
public:
  TextWidget(const std::string &id, const sf::FloatRect &zone,
	     const sf::Text &text = sf::Text(), int maxSize = 2048);

  int		update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
  void		draw(sf::RenderWindow &window) const;
  void		setColor(const sf::Color &color);
  Cursor	&getCursor();

private:
  void	setDrawableText();

private:
  Cursor	_cursor;
  bool		_isActive;
  sf::Text	_textContent;
};

#endif /* _TEXTWIDGET_H_ */
