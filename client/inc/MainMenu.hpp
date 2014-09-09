#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "APanelScreen.hpp"

class MainMenu : public APanelScreen
{
public:
  MainMenu();
  virtual ~MainMenu();

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);

private:
  void		createPlayButton(const sf::Texture &texture, Widget *);
  void		createOptButton(const sf::Texture &texture, Widget *);
  void		createQuitButton(const sf::Texture &texture, Widget *);
};

#endif /* _MAINMENU_H_ */
