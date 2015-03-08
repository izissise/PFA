#include "FastMath.h"

float	pointDist(unsigned int dx, unsigned int dy)
{
  return (std::sqrt(dx * dx + dy * dy));
}

float	pointDistf(float dx, float dy)
{
  return (std::sqrt(dx * dx + dy * dy));
}
