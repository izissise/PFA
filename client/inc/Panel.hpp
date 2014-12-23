#ifndef _PANEL_H_
#define _PANEL_H_

#include "APanelScreen.hpp"

class Panel : public APanelScreen
{
public:
  Panel(const sf::FloatRect &zone);
  virtual ~Panel();

  int		update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);
  void		trigger(const t_event &event);
};

#endif /* _PANEL_H_ */
