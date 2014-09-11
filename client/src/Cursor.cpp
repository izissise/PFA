#include "Cursor.hpp"

Cursor::Cursor(const sf::Text &text, float blink) :
  _cursor(text), _time(), _blinkTime(blink), _display(false)
{
}

Cursor::~Cursor()
{
}

void	Cursor::setCursorPos(float x, float y)
{
  _cursor.setPosition(x, y);
}

void		Cursor::setCursorPos(const sf::Text &text)
{
  sf::FloatRect	textSize = text.getLocalBounds();
  sf::Vector2f	textPos = text.getPosition();

  _cursor.setPosition(textSize.width + textPos.x, textPos.y);
}

void	Cursor::setText(const sf::Text &text)
{
  _cursor = text;
}

void	Cursor::update()
{
  if (_time.getElapsedTime().asMilliseconds() > _blinkTime)
    {
      if (_time.getElapsedTime().asMilliseconds() > 2.0 * _blinkTime)
	{
	  _time.restart();
	  _display = true;
	}
      else
	_display = false;
    }
  else
    _display = true;
}

void	Cursor::draw(sf::RenderWindow &window) const
{
  if (_display)
    window.draw(_cursor);
}
