#include "Cursor.hpp"

Cursor::Cursor(const sf::Text &text, unsigned int blinkTime) :
  BlinkText(blinkTime),
  _cursor(text)
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
  sf::FloatRect	textSize = text.getGlobalBounds();
  sf::Vector2f	textPos = text.getPosition();

  _cursor.setPosition(textSize.width + textPos.x, textPos.y);
}

void	Cursor::setText(const sf::Text &text)
{
  _cursor = text;
}

void	Cursor::update()
{
  BlinkText::update();
}

void	Cursor::draw(sf::RenderWindow &window) const
{
  if (getDisplayState())
    window.draw(_cursor);
}

void	Cursor::draw(sf::RenderTexture &window) const
{
  if (getDisplayState())
    window.draw(_cursor);
}

std::size_t	Cursor::getWidth() const
{
  return _cursor.getGlobalBounds().width;
}

unsigned int	Cursor::getSize() const
{
  return _cursor.getString().getSize();
}

void	Cursor::setColor(const sf::Color &color)
{
  _cursor.setColor(color);
}

void	Cursor::scale(float ratioX, float ratioY)
{
  sf::Vector2f	cursorPos(_cursor.getPosition());

  _cursor.scale(ratioX, ratioY);
  cursorPos.x *= ratioX;
  cursorPos.y *= ratioY;
  _cursor.setPosition(cursorPos);
}
