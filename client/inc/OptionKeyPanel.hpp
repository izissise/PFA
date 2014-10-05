#ifndef _OPTIONKEYPANEL_H_
#define _OPTIONKEYPANEL_H_

#include "APanelScreen.hpp"
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
  void		createOptPanel(const sf::Texture &texture, Widget *);
  void		createKeyWidget(const sf::Texture &texture, KeyWidget *wKey);
  void		createScrollBar(const sf::Texture &texture, ScrollWidget *wKey);
  void		createTextWidget(Widget *widget);
};


#endif /* _OPTIONKEYPANEL_H_ */
