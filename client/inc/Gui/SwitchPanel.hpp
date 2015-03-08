#ifndef _SWITCHPANEL_H_
#define _SWITCHPANEL_H_

#include "ASwitchElem.hpp"

class SwitchPanel : public ASwitchElem
{
public:
  SwitchPanel(const sf::FloatRect &panelZone,
	      const sf::FloatRect &contentZone);

  int	update(const sf::Event &event,
	       sf::RenderWindow &ref,
	       Settings &set);
  void	print(sf::RenderTarget &window, bool isTextured);
  void	addContent(APanelScreen * const widget);
  void	addContent(const std::initializer_list<APanelScreen * const> &widgets);

private:
  unsigned int	getContentSize() const;
};

#endif /* _SWITCHPANEL_H_ */
