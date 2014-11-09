#ifndef _CHECKBOXWIDGET_H_
#define _CHECKBOXWIDGET_H_

#include "AWidget.hpp"

class CheckBoxWidget : public AWidget
{
public:
  CheckBoxWidget(const std::string &id, const sf::FloatRect &zone,
		 const sf::Text &text = sf::Text(),
		 wFlag flg = wFlag::Movable | wFlag::Resizable);

  int	update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
};

#endif /* _CHECKBOXWIDGET_H_ */
