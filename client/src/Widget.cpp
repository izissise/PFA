#include "Widget.hpp"

Widget::Widget(const std::string &id, const sf::FloatRect &zone, sf::Text *text) :
  AWidget(id, zone, text)
{
}
#include <iostream>
int	Widget::update(const sf::Event &event, const sf::RenderWindow &ref)
{
  if (isOver(ref) && isClicked(event, sf::Mouse::Left))
    std::cout << "Widget " + _id + " is clicked" << std::endl;
}
