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

  void			draw(sf::RenderWindow &win) const;
  void			clear();
  const	sf::Vertex	&getPoint(unsigned int idx);
  unsigned int		size() const;

  sf::VertexArray	points;
};

#endif /* _LINES_H_ */
