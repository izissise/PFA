#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "APanelScreen.hpp"

class MainMenu : public APanelScreen
{
public:
  MainMenu(const sf::Texture &texture, Settings &set);
  virtual ~MainMenu();

  int		run(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
};

#endif /* _MAINMENU_H_ */
