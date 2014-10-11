#ifndef _GAMEPANEL_H_
#define _GAMEPANEL_H_

#include "APanelScreen.hpp"
#include "Lines.hpp"

#define ITERATIONS 5
#define MHEIGHT 400

class GamePanel : public APanelScreen
{
public:
  GamePanel(const sf::FloatRect &zone);
  virtual ~GamePanel();

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);

  void		draw(sf::RenderWindow &window, bool toWin);
private:
  int		fastPow(int x, int it) const;
  void		constructLine();
  void		drawHud(sf::RenderWindow &window, bool toWin);

  Lines		_line;
};

#endif /* _GAMEPANEL_H_ */
