#ifndef _OPTIONGAMEPANEL_H_
#define _OPTIONGAMEPANEL_H_

#include "APanelScreen.hpp"
#include "Widget.hpp"
#include "TextWidget.hpp"
#include "BarWidget.hpp"

class OptionGamePanel : public APanelScreen
{
public:
  OptionGamePanel();
  virtual ~OptionGamePanel();

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);

private:
  void		createOptPanel(const sf::Texture &texture, Widget *);
  void		createCheckBox(const sf::Texture &texture, Widget *);
  void		createTextWidget(const sf::Texture &texture, TextWidget *);

};

#endif /* _OPTIONGAMEPANEL_H_ */
