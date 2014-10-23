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

void	Lines::update(const sf::Vector2f &padding)
{
  if (_line.getVertexCount() > 0)
    _line.clear();
  for (auto &p : points)
    _line.append(sf::Vertex(p.position + padding));
}

void	Lines::draw(sf::RenderWindow &win) const
{
  win.draw(_line);
}

const sf::Vertex	&Lines::getPointFromList(unsigned int idx)
{
  auto it = points.begin();

  std::advance(it, idx);
  return *it;
}

const sf::Vertex	&Lines::getPoint(unsigned int idx)
{
  return _line[idx];
}

void	Lines::clear()
{
  points.clear();
  _line.clear();
}

unsigned int	Lines::size() const
{
  return _line.getVertexCount();
}
