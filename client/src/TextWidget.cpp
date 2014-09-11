#include "TextWidget.hpp"
#include "Unused.hpp"

TextWidget::TextWidget(const std::string &id, const sf::FloatRect &zone,
		       const sf::Text &text, int maxSize) :
  AWidget(id, zone, text), UserInput(maxSize), _isActive(false)
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
      _text.setString(getString());
      _cursor.update();
      _cursor.setCursorPos(_text);
      for (auto &func : _updates)
	{
	  if ((retVal = func.second(*this, event, ref)) != 0)
	    return retVal;
	}
    }
  return catched;
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
