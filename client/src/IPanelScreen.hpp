#ifndef _IPANELSCREEN_H_
#define _IPANELSCREEN_H_

#include <SFML/Graphics.hpp>

class IPanelScreen
{
public:
  IPanelScreen();
  virtual ~IPanelScreen() = 0;

  virtual int	run(sf::Event event) = 0;
  virtual void	draw(sf::RenderWindow &window) = 0;
};

#endif /* _IPANELSCREEN_H_ */
