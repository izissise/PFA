#include "Widget.hpp"

Widget::Widget(const std::string &id, const sf::FloatRect &zone,
	       const sf::Text &text) :
  AWidget(id, zone, text)
{
  _updates["main"] = [](AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)
    -> int
    {
      return 0;
    };
}

int		Widget::update(const sf::Event &event, sf::RenderWindow &ref, Settings &set)
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
