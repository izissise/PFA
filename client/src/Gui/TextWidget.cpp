#include <cmath>
#include "TextWidget.hpp"
#include "Unused.hpp"

TextWidget::TextWidget(const std::string &id, const sf::FloatRect &zone,
		       const sf::Text &text, const sf::Text &def,
		       int maxSize, wFlag flg) :
  AWidget(id, zone, text, flg), UserInput(maxSize), _isActive(false),
  _textContent(text), _default(def)
{
  sf::Text	curText(text);

  curText.setString("|");
  _cursor.setText(curText);
}

int	TextWidget::setToDefault(const sf::Event &event, sf::RenderWindow &ref)
{
  if (_text.getString().getSize() == 0)
    {
      _text.setString(_default.getString());
      _text.setColor(_default.getColor());
      if (_update)
	if (_update(*this, event, ref) != 0)
	  return 1;
    }
  return 0;
}

int	TextWidget::update(const sf::Event &event, sf::RenderWindow &ref,
			   Settings &set UNUSED)
{
  int	catched = 0;
  int	retVal = 0;

  if (_hide == true) // some fuckery
    {
      _update(*this, event, ref);
      return 0;
    }
  if (isClicked(event, sf::Mouse::Left))
    {
      _isActive = isOver(ref);
      catched = _isActive;
      if (!_isActive)
	retVal = setToDefault(event, ref);
      else
	_text.setColor(_textContent.getColor());
    }
  if (_isActive)
    {
      catched = _isActive;
      _isActive = !getInput(event);
      if (!_isActive)
	{
	  setToDefault(event, ref);
	  if (_update)
	    retVal = _update(*this, event, ref);
	}
      else
	{
	  _textContent.setString(getString());
	  setDrawableText();
	  if (_update)
	    retVal = _update(*this, event, ref);
	  _cursor.update();
	  _cursor.setCursorPos(_text);
	}
      if (retVal)
	return retVal;
    }
  else
    setToDefault(event, ref);
  return catched;
}

void		TextWidget::setColor(const sf::Color &color)
{
  _textContent.setColor(color);
}

void		TextWidget::setDefaultColor(const sf::Color &color)
{
  _default.setColor(color);
}

Cursor		&TextWidget::getCursor()
{
  return _cursor;
}

void		TextWidget::setDrawableText()
{
  sf::FloatRect	bounds;
  sf::String	str;
  std::size_t	size;
  int		charSize;

  bounds = _textContent.getGlobalBounds();
  _text.setString(_textContent.getString());
  if (_text.getString().getSize() <= 1)
    return ;
  str = _textContent.getString();
  size = str.getSize() + _cursor.getSize();
  charSize = (bounds.width + _cursor.getWidth()) / size;
  if (bounds.width + _cursor.getWidth() + charSize > _zone.width)
    {
      str.erase(0, std::ceil((bounds.width + _cursor.getWidth()
			      + charSize - _zone.width) / charSize));
      _text.setString(str);
    }
}

void		TextWidget::draw(sf::RenderTarget &window) const
{
  AWidget::draw(window);
  if (_isActive)
    _cursor.draw(window);
}

std::string	TextWidget::getContent() const
{
  return _text.getString().toAnsiString();
}

void		TextWidget::scaleText(sf::Text &text, float ratioX, float ratioY)
{
  sf::Vector2f	textPos(text.getPosition());

  text.scale(ratioX, ratioY);
  textPos.x *= ratioX;
  textPos.y *= ratioY;
  text.setPosition(textPos);
}

void	TextWidget::scale(const sf::Vector2f &size)
{
  float	ratioX = size.x / SIZEX;
  float	ratioY = size.y / SIZEY;

  sf::Vector2f defaultPos(_default.getPosition());
  sf::Vector2f contentPos(_textContent.getPosition());
  sf::Vector2f spritePos;

  _zone.left *= ratioX;
  _zone.top *= ratioY;
  _zone.width *= ratioX;
  _zone.height *= ratioY;
  for (auto &elem : _sprites)
    {
      spritePos = elem.sprite.getPosition();
      spritePos.x *= ratioX;
      spritePos.y *= ratioY;
      elem.sprite.setPosition(spritePos);
      elem.sprite.scale(ratioX, ratioY);
    }
  scaleText(_text, ratioX, ratioY);
  scaleText(_default, ratioX, ratioY);
  scaleText(_textContent, ratioX, ratioY);
  _cursor.scale(ratioX, ratioY);
}

void	TextWidget::resize(float pX, float pY)
{
  _zone.width *= pX;
  _zone.height *= pY;
  for (auto &elem : _sprites)
    {
      elem.sprite.scale(pX, pY);
    }
}

void	TextWidget::clearWidget()
{
  _textContent.setString("");
  _text.setString("");
  UserInput::clear(); // clear userInput
  _cursor.update();
  _cursor.setCursorPos(_text);
}

void	TextWidget::trigger(const t_event &event)
{
  if (event.e & wEvent::Reset)
    clearWidget();
  else
    AWidget::trigger(event);
}

bool	TextWidget::getState() const
{
  return _isActive;
}

void	TextWidget::setState(bool state)
{
  _isActive = state;
  _text.setColor(_textContent.getColor());
}
