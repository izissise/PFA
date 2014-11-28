#ifndef _SELECTLIST_H_
#define _SELECTLIST_H_

#include "ASelectList.hpp"
#include "ScrollWidget.hpp"

class SelectList : public ASelectList
{
public:
  SelectList(const sf::FloatRect &zone);

  void	construct(const sf::Texture &texture, Settings &set,
		  const std::vector<APanelScreen *> &panels);
  void	trigger(const t_event &event);

private:
  void	createHeader(const sf::Texture &texture, Widget *w);
  void	createButton(const sf::Texture &texture, Widget *w);
  void	createScroll(const sf::Texture &texture, ScrollWidget *wScroll);
};

#endif /* _SELECTLIST_H_ */
