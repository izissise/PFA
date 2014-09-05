#include "Widget.hpp"

Widget::Widget(const std::string &id, const sf::Vector2u &pos, sf::Text *text) :
  AWidget(id, pos, text)
{
}

int	Widget::update(sf::Event event)
{
}
