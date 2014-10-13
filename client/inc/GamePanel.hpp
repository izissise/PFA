#ifndef _GAMEPANEL_H_
#define _GAMEPANEL_H_

#include "APanelScreen.hpp"
#include "World.hpp"

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

  int		_pad;
  int		_padup;
  int		_oldY;
  bool		_dir;
  std::unique_ptr<World>	_world;
};

#endif /* _GAMEPANEL_H_ */
