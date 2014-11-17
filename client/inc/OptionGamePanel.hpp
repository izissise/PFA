#ifndef _OPTIONGAMEPANEL_H_
#define _OPTIONGAMEPANEL_H_

#include "Widget.hpp"
#include "TextWidget.hpp"
#include "BarWidget.hpp"
#include "ScrollWidget.hpp"
#include "SelectList.hpp"
#include "CheckBoxWidget.hpp"
#include "SwitchPanel.hpp"

class OptionGamePanel : public APanelScreen
{
public:
  OptionGamePanel(const sf::FloatRect &zone);
  virtual ~OptionGamePanel();

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);

private:
  void		createOptPanel(const sf::Texture &texture, Widget *);
  void		createCheckBox(const sf::Texture &texture, CheckBoxWidget *);
  void		createTextWidget(const sf::Texture &texture, TextWidget *);
  void		createScrollWidget(const sf::Texture &texture, ScrollWidget *wScroll);
  void		createSwitchPanel(const sf::Texture &texture, SwitchPanel *pSwitch);
};

#endif /* _OPTIONGAMEPANEL_H_ */
