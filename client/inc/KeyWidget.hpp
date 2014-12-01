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
  void		trigger(const t_event &event);

private:
  void	bindKey(Settings &set);
  void	unbindKey(Settings &set);
  void	getKeyName(const Controls &ctrl);

private:
  bool		_isActive;
  Action	_action;
  t_entry	_entry;
  bool		_forceUpdate;
};

#endif /* _KEYWIDGET_H_ */