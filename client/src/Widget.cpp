#include "Widget.hpp"

Widget::Widget(const std::string &id, const sf::FloatRect &zone,
	       const sf::Text &text) :
  AWidget(id, zone, text)
{
  _updates["main"] = [](AWidget &widget, const sf::Event &event, sf::RenderWindow &ref) -> int
    {
      return 0;
    };
}

int	Widget::update(const sf::Event &event, sf::RenderWindow &ref, Settings &set)
{
  for (auto &func : _updates)
    func.second(*this, event, ref);
  return 0;
}
