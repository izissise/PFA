#include "KeyWidget.hpp"
#include "Exception.hpp"

KeyWidget::KeyWidget(const std::string &id, const sf::FloatRect &zone,
		     Action act, const Controls &ctrl, const sf::Text &text):
  AWidget(id, zone, text), _isActive(false), _action(act)
{
  getKeyName(ctrl);
}

void	KeyWidget::getKeyName(const Controls &ctrl)
{
  _entry = ctrl.getLastKey(_action);
  try
    {
      _text.setString(ctrl.getCodeFromKey(_entry));
    }
  catch (Exception &e)
    {
      _text.setString("Unset");
    }
}

void	KeyWidget::bindKey(Settings &set)
{
  try
    {
      _text.setString(set.getControls().getCodeFromKey(_entry));
      set.getControls().bindKeyOnAction(_entry, _action);
    }
  catch (Exception &e)
    {
      _text.setString("Unknown");
    }
}

void	KeyWidget::unbindKey(Settings &set)
{
  Controls	&ctrl = set.getControls();

  ctrl.unbindKeyFromAction(ctrl.getLastKey(_action), _action);
  getKeyName(ctrl);
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
	  if (_entry.type == ctrl::type::Keyboard && _entry.key == sf::Keyboard::Escape)
	    unbindKey(set);
	  else
	    bindKey(set);
	}
    }
  if (_update)
    {
      if (_update(*this, event, ref) != 0)
	return 1;
    }
  return retVal;
}

