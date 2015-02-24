#ifndef _PANEL_H_
#define _PANEL_H_

#include "APanelScreen.hpp"

class Panel : public APanelScreen
{
public:
  Panel(const sf::FloatRect &zone);
  virtual ~Panel();

  Panel(const Panel &rhs) = delete;
  Panel     &operator=(const Panel &rhs) = delete;

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);
  void		trigger(const t_event &event);
};

#endif /* _PANEL_H_ */
