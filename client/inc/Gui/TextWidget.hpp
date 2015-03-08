#ifndef _TEXTWIDGET_H_
#define _TEXTWIDGET_H_

#include "AWidget.hpp"
#include "UserInput.hpp"
#include "Cursor.hpp"

class TextWidget : public AWidget, public UserInput
{
public:
  TextWidget(const std::string &id, const sf::FloatRect &zone,
	     const sf::Text &text = sf::Text(), const sf::Text &def = sf::Text(),
	     int maxSize = 2048,
	     wFlag flg = wFlag::Movable | wFlag::Resizable | wFlag::Getable);

  int		update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
  void		draw(sf::RenderTarget &window) const;
  std::string	getContent() const;
  void		scale(const sf::Vector2f &size);
  void		resize(float pX, float pY);
  void		setColor(const sf::Color &color);
  void		setDefaultColor(const sf::Color &color);
  Cursor	&getCursor();
  void		trigger(const t_event &event);
  bool		getState() const;
  void		setState(bool state);
  void		clearWidget();

private:
  void	setDrawableText();
  void	scaleText(sf::Text &text, float ratioX, float ratioY);
  int	setToDefault(const sf::Event &event, sf::RenderWindow &ref);

private:
  Cursor	_cursor;
  bool		_isActive;
  sf::Text	_textContent;
  sf::Text	_default;
};

#endif /* _TEXTWIDGET_H_ */
