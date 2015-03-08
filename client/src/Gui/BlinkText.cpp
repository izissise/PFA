#include "BlinkText.hpp"

BlinkText::BlinkText(unsigned int blinkTime) :
  _time(),
  _blinkTime(blinkTime),
  _display(true)
{
}

BlinkText::~BlinkText()
{
}

void	BlinkText::update()
{
  if (_time.getElapsedTime().asMilliseconds() > static_cast<int>(_blinkTime))
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

bool	BlinkText::getDisplayState() const
{
  return _display;
}
