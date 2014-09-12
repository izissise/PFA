#include <cmath>
#include "TextWidget.hpp"
#include "Unused.hpp"

TextWidget::TextWidget(const std::string &id, const sf::FloatRect &zone,
		       const sf::Text &text, int maxSize) :
  AWidget(id, zone, text), UserInput(maxSize), _isActive(false), _textContent(text)
{
  sf::Text	curText(text);

  curText.setString("|");
  _cursor.setText(curText);
}

int	TextWidget::update(const sf::Event &event, sf::RenderWindow &ref,
			   Settings &set UNUSED)
{
  int	catched = 0;
  int	retVal;

  if (_hide)
    return 0;
  if (isClicked(event, sf::Mouse::Left))
    {
      _isActive = isOver(ref);
      catched = _isActive;
    }
  if (_isActive)
    {
      _isActive = !getInput(event);
      _textContent.setString(getString());
      setDrawableText();
      for (auto &func : _updates)
	{
	  if ((retVal = func.second(*this, event, ref)) != 0)
	    return retVal;
	}
      _cursor.update();
      _cursor.setCursorPos(_text);
    }
  return catched;
}

void		TextWidget::setColor(const sf::Color &color)
{
  _text.setColor(color);
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
  str = _textContent.getString();
  size = str.getSize() + _cursor.getSize();
  charSize = (bounds.width + _cursor.getWidth()) / size;
  if (bounds.width + _cursor.getWidth() + charSize > _zone.width)
    {
      str.erase(0, std::ceil((bounds.width + _cursor.getWidth() + charSize - _zone.width) / charSize));
      _text.setString(str);
    }
}

void	TextWidget::draw(sf::RenderWindow &window) const
{
  if (_hide)
    return ;
  for (auto &elem : _sprites)
    if (elem.draw)
      window.draw(elem.sprite);
  window.draw(_text);
  if (_isActive)
    _cursor.draw(window);
}
