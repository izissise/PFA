#ifndef _TABWIDGET_H_
#define _TABWIDGET_H_

#include "APanelScreen.hpp"

class TabWidget : public AWidget
{
public:
  TabWidget(const std::string &id, const sf::FloatRect &zone,
	    const sf::Text &text = sf::Text(), APanelScreen *panel = nullptr);

  int	update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
  void	trigger(const t_event &event);

  APanelScreen	*getPanel() const;

private:
  APanelScreen		*_panel;
};

#endif /* _TABWIDGET_H_ */
