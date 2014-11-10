#ifndef _OPTIONKEYPANEL_H_
#define _OPTIONKEYPANEL_H_

#include "Panel.hpp"
#include "Widget.hpp"
#include "TextWidget.hpp"
#include "KeyWidget.hpp"
#include "ScrollWidget.hpp"

class OptionKeyPanel : public APanelScreen
{
public:
  OptionKeyPanel(const sf::FloatRect &zone);
  virtual ~OptionKeyPanel();

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);

private:
  void		createTabs(Widget *widget);
  void		createContPanel(Settings &set, const sf::Texture &texture);
  void		createEventPanel(Panel *eventPanel, const sf::FloatRect &zone,
				 Settings &set, const sf::Texture &texture);
  void		createKeyPanel(Panel *eventPanel, const sf::FloatRect &zone,
			       Settings &set, const sf::Texture &texture);
  void		fillOptionBg(const sf::Texture &texture, Widget *);
  void		createKeyWidget(const sf::Texture &texture, KeyWidget *wKey);
  void		createScrollBar(const sf::Texture &texture, ScrollWidget *wKey);
  void		createTextWidget(Widget *widget);
};


#endif /* _OPTIONKEYPANEL_H_ */
