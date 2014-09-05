#include "Widget.hpp"

Widget::Widget(const std::string &id, const sf::Vector2f &pos, sf::Text *text) :
  AWidget(id, pos, text)
{
}
#include <iostream>
int	Widget::update(sf::Event event)
{
  if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Right)
	{
	  std::cout << "the right button was pressed" << std::endl;
	  std::cout << "mouse x: " << event.mouseButton.x << std::endl;
	  std::cout << "mouse y: " << event.mouseButton.y << std::endl;
	}
    }

}
