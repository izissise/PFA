#ifndef _OPTIONTABPANEL_H_
#define _OPTIONTABPANEL_H_

#include "APanelScreen.hpp"
#include "TabWidget.hpp"

class OptionTabPanel : public APanelScreen
{
public:
  OptionTabPanel();
  virtual ~OptionTabPanel();

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);
  void		trigger(const t_event &event);

private:
  void		createBarGame(const sf::Texture &texture, TabWidget *);
  void		createBarKeyboard(const sf::Texture &texture, TabWidget *);
};

#endif /* _OPTIONPANEL_H_ */
