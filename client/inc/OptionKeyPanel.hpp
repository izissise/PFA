#ifndef _OPTIONKEYPANEL_H_
#define _OPTIONKEYPANEL_H_

#include "APanelScreen.hpp"
#include "Widget.hpp"

class OptionKeyPanel : public APanelScreen
{
public:
  OptionKeyPanel();
  virtual ~OptionKeyPanel();

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);

private:
  void		createOptPanel(const sf::Texture &texture, Widget *);
};


#endif /* _OPTIONKEYPANEL_H_ */
