#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "APanelScreen.hpp"
#include "Widget.hpp"

class MainMenu : public APanelScreen
{
public:
  MainMenu(const sf::FloatRect &zone);
  virtual ~MainMenu() = default;

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);

private:
  void		createBgPanel(const sf::Texture &texture, Widget *wBackground,
			      Widget *wMback, Widget *wMenuTitle);
  void		createPlayButton(Widget *);
  void		createOptButton(Widget *);
  void		createQuitButton(Widget *);
  void		createButtonStyle(Widget *);
};

#endif /* _MAINMENU_H_ */
