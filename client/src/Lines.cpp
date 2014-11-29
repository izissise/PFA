#include "Lines.hpp"

Lines::Lines() :
  points(sf::LinesStrip)
{
}

Lines::~Lines()
{
}

void	Lines::draw(sf::RenderWindow &win) const
{
  win.draw(points);
}

const sf::Vertex	&Lines::getPoint(unsigned int idx)
{
  return points[idx];
}

void	Lines::clear()
{
  points.clear();
}

unsigned int	Lines::size() const
{
  return points.getVertexCount();
}
