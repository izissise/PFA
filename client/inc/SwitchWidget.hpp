#ifndef _SWITCHWIDGET_H_
#define _SWITCHWIDGET_H_

#include "ASwitchElem.hpp"

class SwitchWidget : public ASwitchElem
{
public:
  SwitchWidget(const sf::FloatRect &panelZone,
	       const sf::FloatRect &contentZone);

  int	update(const sf::Event &event,
	       sf::RenderWindow &ref,
	       Settings &set);
  void	draw(sf::RenderTarget &window, bool toWin);
  void	addContent(AWidget * const widget);
  void	addContent(const std::initializer_list<AWidget * const> &widgets);

private:
  unsigned int	getContentSize() const;
};

#endif /* _SWITCHWIDGET_H_ */
