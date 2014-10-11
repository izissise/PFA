#ifndef _LINES_H_
#define _LINES_H_

#include <SFML/Graphics.hpp>
#include <list>
#include <iterator>

class Lines
{
public:
  Lines();
  virtual ~Lines();

  std::list<sf::Vertex>	&getPoints();
  void			update();
  void			draw(sf::RenderWindow &win) const;
  const sf::Vertex	&getPoint(unsigned int idx);

  std::list<sf::Vertex>	points;
private:
  sf::VertexArray	_line;
};

#endif /* _LINES_H_ */
