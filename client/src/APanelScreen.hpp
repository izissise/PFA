#ifndef _APANELSCREEN_H_
#define _APANELSCREEN_H_

#include <vector>
#include "Widget.hpp"

class APanelScreen
{
public:
  APanelScreen(sf::Texture * const texture, sf::Texture * const background);
  virtual ~APanelScreen() = 0;

  virtual int	run(sf::Event event) = 0;
  virtual void	draw(sf::RenderWindow &window);

protected:
  sf::Font			_font;
  sf::Texture			*_background;
  sf::Texture			*_texture;
  std::vector<AWidget *>	_widgets;
};

#endif /* _APANELSCREEN_H_ */
