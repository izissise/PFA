#ifndef _GAMEPANEL_H_
#define _GAMEPANEL_H_

#include "APanelScreen.hpp"
#include "Lines.hpp"

#define ITERATIONS 5
#define MHEIGHT 300
#define SHEIGHT 500.f

class GamePanel : public APanelScreen
{
public:
  GamePanel(const sf::FloatRect &zone);
  virtual ~GamePanel();

  void		construct(const sf::Texture &texture, Settings &set,
			  const std::vector<APanelScreen *> &panels);

  void		draw(sf::RenderWindow &window, bool toWin);
  int		update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
private:
  void		constructLine();
  void		completeField(sf::RenderWindow &window);
  void		fillVertex(sf::Vector2f &prev, sf::Vector2f &next, int x);
  void		drawHud(sf::RenderWindow &window, bool toWin);

  std::array<sf::Sprite, 2>	_sprites;
  int		_pad;
  int		_padup;
  int		_oldY;
  bool		_dir;
  Lines		_line;
};

#endif /* _GAMEPANEL_H_ */
