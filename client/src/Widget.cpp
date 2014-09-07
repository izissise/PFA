#include "Widget.hpp"

Widget::Widget(const std::string &id, const sf::FloatRect &zone, sf::Text *text) :
  AWidget(id, zone, text)
{
  _update = [](AWidget &widget, const sf::Event &event, sf::RenderWindow &ref) -> int
    {
      return 0;
    };
}

int	Widget::update(const sf::Event &event, sf::RenderWindow &ref, Settings &set)
{
  return (_update(*this, event, ref));
}
