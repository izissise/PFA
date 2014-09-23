#include "TabWidget.hpp"
#include "Unused.hpp"

TabWidget::TabWidget(const std::string &id, const sf::FloatRect &zone,
		     const sf::Text &text, APanelScreen *panel) :
  AWidget(id, zone, text), _panel(panel)
{
}

APanelScreen	*TabWidget::getPanel() const
{
  return _panel;
}

int	TabWidget::update(const sf::Event &event, sf::RenderWindow &ref, Settings &set UNUSED)
{
  int	retVal = 0;

  if (_hide)
    return 0;
  for (auto &func : _updates)
    {
      if ((retVal = func.second(*this, event, ref)) != 0)
	return retVal;
    }
  return retVal;
}

void	TabWidget::trigger(const t_event &event)
{
  if (event.e & wEvent::SetSprite)
    {
      setSpriteAttr(event.idx, event.value);
      if (event.idx == 1 && event.value == 0)
	_panel->setHide(true);
    }
  else if (event.e & wEvent::Hide)
    {
      if (event.e & wEvent::Toggle)
	_hide = !_hide;
      else
	_hide = true;
    }
}
