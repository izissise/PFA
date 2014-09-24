#include "KeyWidget.hpp"
#include "Exception.hpp"

KeyWidget::KeyWidget(const std::string &id, const sf::FloatRect &zone,
		     Action act, const Controls &ctrl, const sf::Text &text):
  AWidget(id, zone, text), _isActive(false), _action(act)
{
  _k = ctrl.getKeyFromAction(act);
  try
    {
      _text.setString(ctrl.getCodeFromKey(_k));
    }
  catch (Exception &e)
    {
      _text.setString("");
    }
}

int	KeyWidget::update(const sf::Event &event, sf::RenderWindow &ref, Settings &set)
{
  int	retVal = 0;

  if (_hide)
    return 0;
  if (isClicked(event, sf::Mouse::Left))
    {
      _isActive = isOver(ref);
      if (_isActive)
	{
	  retVal = _isActive;
	  _k = sf::Keyboard::Unknown;
	  _text.setString("");
	}
    }
  if (event.type == sf::Event::KeyPressed && _isActive)
    {
      retVal = 1;
      _isActive = false;
      _k = event.key.code;
      try
	{
	  _text.setString(set.getControls().getCodeFromKey(event.key.code));
	}
      catch (Exception &e)
	{
	  _text.setString("");
	}
    }
  for (auto &func : _updates)
    {
      if (func.second(*this, event, ref) != 0)
	return 1;
    }
  return retVal;
}

