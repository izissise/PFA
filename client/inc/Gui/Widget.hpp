#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "AWidget.hpp"

class Widget : public AWidget
{
public:
  Widget(const std::string &id, const sf::FloatRect &zone,
	 const sf::Text &text = sf::Text(),
	 wFlag flg = wFlag::Movable | wFlag::Resizable);

  int	update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
  void	trigger(const t_event &event);
};

#endif /* _WIDGET_H_ */
