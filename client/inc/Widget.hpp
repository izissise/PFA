#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "AWidget.hpp"

class Widget : public AWidget
{
public:
  Widget(const std::string &id, const sf::FloatRect &zone, sf::Text *text = NULL);

  int	update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
};

#endif /* _WIDGET_H_ */
