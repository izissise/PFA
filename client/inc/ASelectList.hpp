#ifndef _ASELECTLIST_H_
#define _ASELECTLIST_H_

#include "Panel.hpp"
#include "Widget.hpp"

class ASelectList : public APanelScreen
{
public:
  ASelectList(const sf::FloatRect &zone);
  virtual ~ASelectList();

  virtual void		construct(const sf::Texture &texture, Settings &set,
				  const std::vector<APanelScreen *> &panels) = 0;
  void		trigger(const t_event &event);
};

#endif /* _ASELECTLIST_H_ */
