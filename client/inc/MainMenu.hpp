#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "APanelScreen.hpp"

class MainMenu : public APanelScreen
{
public:
  MainMenu(const sf::Texture &texture, Settings &set);
  virtual ~MainMenu();

  int		run(const sf::Event &event, sf::RenderWindow &ref, Settings &set);

private:
  void		createPlayButton(const sf::Texture &texture, Widget *);
  void		createOptButton(const sf::Texture &texture, Widget *);
  void		createQuitButton(const sf::Texture &texture, Widget *);
};

#endif /* _MAINMENU_H_ */
