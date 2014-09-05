#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "AWidget.hpp"

class Widget : public AWidget
{
public:
  Widget(const std::string &id, const sf::Vector2u &pos, sf::Text *text = NULL);

  int	update(sf::Event event);
};

#endif /* _WIDGET_H_ */
