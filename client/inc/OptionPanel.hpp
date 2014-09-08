#ifndef _OPTIONPANEL_H_
#define _OPTIONPANEL_H_

#include "APanelScreen.hpp"

class OptionPanel : public APanelScreen
{
public:
  OptionPanel();
  virtual ~OptionPanel();

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);
  int		run(const sf::Event &event, sf::RenderWindow &ref, Settings &set);

private:
  void		createOptPanel(const sf::Texture &texture, Widget *);
  void		createBarGame(const sf::Texture &texture, Widget *);
  void		createBarKeyboard(const sf::Texture &texture, Widget *);
};

#endif /* _OPTIONPANEL_H_ */
