#include "Widget.hpp"
#include "Unused.hpp"

Widget::Widget(const std::string &id, const sf::FloatRect &zone,
	       const sf::Text &text, wFlag flg) :
  AWidget(id, zone, text, flg)
{
  _update = [](AWidget &widget UNUSED,
	       const sf::Event &event UNUSED,
	       sf::RenderWindow &ref UNUSED)
    -> int
    {
      return 0;
    };
}

int		Widget::update(const sf::Event &event, sf::RenderWindow &ref, Settings &set UNUSED)
{
  if (_hide || !_update)
    return 0;
  return _update(*this, event, ref);
}

void		Widget::trigger(const t_event &event)
{
  if (_event)
    _event(event);
  else
    AWidget::trigger(event);
}
