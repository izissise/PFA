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
  bool	over;

  if (_update)
    {
      if (_update(*this, event, ref) != 0)
	return 1;
    }
  else
    {
      over = isOver(ref);
      if (over)
	{
	  if (isClicked(event, sf::Mouse::Left) && getSpriteAttr(0).draw == true)
	    {
	      toggleSpriteAttr(0);
	      toggleSpriteAttr(1);
	      getPanel()->setHide(false);
	      notify(t_event(wEvent::SetSprite, 0, 1));
	      notify(t_event(wEvent::SetSprite, 1, 0));
	      return 1;
	    }
	}
    }
  return 0;
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
