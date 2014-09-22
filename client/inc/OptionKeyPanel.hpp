#ifndef _OPTIONKEYPANEL_H_
#define _OPTIONKEYPANEL_H_

#include "APanelScreen.hpp"
#include "Widget.hpp"
#include "TextWidget.hpp"

class OptionKeyPanel : public APanelScreen
{
public:
  OptionKeyPanel();
  virtual ~OptionKeyPanel();

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);

private:
  void		createOptPanel(const sf::Texture &texture, Widget *);
  void		createKeyWidget(const sf::Texture &texture, TextWidget *, Action act);
};


#endif /* _OPTIONKEYPANEL_H_ */
