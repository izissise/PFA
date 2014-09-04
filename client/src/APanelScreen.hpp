#ifndef _APANELSCREEN_H_
#define _APANELSCREEN_H_

#include <map>
#include "IPanelScreen.hpp"

class APanelScreen
{
public:
  APanelScreen(sf::Texture * const texture);
  virtual ~APanelScreen() = 0;

  virtual int	run(sf::Event event) = 0;
  virtual void	draw(sf::RenderWindow &window);

private:
  sf::Text	_text;
  sf::Texture	*_texture;
  std::map<std::string, sf::VertexArray>	_sprites;
};

#endif /* _APANELSCREEN_H_ */
