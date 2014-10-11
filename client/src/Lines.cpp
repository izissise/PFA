#include "Lines.hpp"

Lines::Lines() :
  _line(sf::LinesStrip)
{
}

Lines::~Lines()
{
}

void	Lines::update()
{
  if (_line.getVertexCount() > 0)
    _line.clear();
  for (auto &p : points)
    _line.append(p);
}

void	Lines::draw(sf::RenderWindow &win) const
{
  win.draw(_line);
}

const sf::Vertex	&Lines::getPoint(unsigned int idx)
{
  auto it = points.begin();

  std::advance(it, idx);
  return *it;
}

void	Lines::clear()
{
  points.clear();
  _line.clear();
}
