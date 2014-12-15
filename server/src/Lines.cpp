#include "Lines.hpp"

Lines::Lines()
{
}

Lines::~Lines()
{
}

const Vector2f	&Lines::getPoint(unsigned int idx)
{
  return points[idx];
}

void	Lines::clear()
{
  points.clear();
}

unsigned int	Lines::size() const
{
  return points.size();
}
