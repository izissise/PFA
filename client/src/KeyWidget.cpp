#include "KeyWidget.hpp"
#include "Exception.hpp"

KeyWidget::KeyWidget(const std::string &id, const sf::FloatRect &zone,
		     Action act, const Controls &ctrl, const sf::Text &text):
  AWidget(id, zone, text), _isActive(false), _action(act)
{
  _entry = ctrl.getLastKey(act);
  try
    {
      _text.setString(ctrl.getCodeFromKey(_entry));
    }
  catch (Exception &e)
    {
      _text.setString("Unknown");
    }
}

int	KeyWidget::update(const sf::Event &event, sf::RenderWindow &ref, Settings &set)
{
  int	retVal = 0;

  if (_hide)
    return 0;
  if (isClicked(event, sf::Mouse::Left) && !_isActive)
    {
      _isActive = isOver(ref);
      if (_isActive)
	{
	  retVal = _isActive;
	  _entry.key = sf::Keyboard::Unknown;
	  _text.setString("");
	}
    }
  else if (_isActive)
    {
      if (event.type == sf::Event::KeyPressed ||
	  event.type == sf::Event::MouseButtonPressed ||
	  event.type == sf::Event::MouseWheelMoved)
	{
	  _entry.fill(event);
	  retVal = 1;
	  _isActive = false;
	  try
	    {
	      _text.setString(set.getControls().getCodeFromKey(_entry));
	    }
	  catch (Exception &e)
	    {
	      _text.setString("Unknown");
	    }
	}
    }
  for (auto &func : _updates)
    {
      if (func.second(*this, event, ref) != 0)
	return 1;
    }
  return retVal;
}

