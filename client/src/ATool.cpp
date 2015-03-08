#include "ATool.hpp"

ATool::ATool(unsigned int damages) :
  _damages(damages)
{
}

unsigned int	ATool::getDamages() const
{
  return _damages;
}
