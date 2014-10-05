#ifndef _PANEL_H_
#define _PANEL_H_

#include "APanelScreen.hpp"

class Panel : public APanelScreen
{
public:
  Panel(const sf::FloatRect &zone);
  virtual ~Panel();

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);
};

#endif /* _PANEL_H_ */
