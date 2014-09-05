#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "APanelScreen.hpp"

class MainMenu : public APanelScreen
{
public:
  MainMenu(sf::Texture * const texture = NULL, sf::Texture * const background = NULL);
  virtual ~MainMenu();

  int		run(sf::Event event);
};

#endif /* _MAINMENU_H_ */
