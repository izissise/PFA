#ifndef _APANELSCREEN_H_
#define _APANELSCREEN_H_

#include <vector>
#include "Widget.hpp"

class APanelScreen
{
public:
  APanelScreen(sf::Texture * const texture);
  virtual ~APanelScreen() = 0;

  virtual int	run(const sf::Event &event, const sf::RenderWindow &ref) = 0;
  virtual void	draw(sf::RenderWindow &window);

protected:
  void		saveTexture(sf::Texture * const texture);
  sf::Texture	*addSpriteForWidget(AWidget * const widget,
				    const sf::Color &color,
				    const sf::Vector2u &size);

protected:
  sf::Font			_font;
  std::vector<sf::Texture *>	_textures;
  std::vector<AWidget *>	_widgets;
};

#endif /* _APANELSCREEN_H_ */
