#ifndef _KEYWIDGET_H_
#define _KEYWIDGET_H_

#include "AWidget.hpp"

class KeyWidget : public AWidget
{
public:
  KeyWidget(const std::string &id, const sf::FloatRect &zone,
	    Action act, const Controls &ctrl,
	    const sf::Text &text = sf::Text());

  int		update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);

private:
  bool		_isActive;
  Action	_action;
  ctrl::key	_k;
};

#endif /* _KEYWIDGET_H_ */
