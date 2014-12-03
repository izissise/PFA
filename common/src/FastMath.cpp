#include "FastMath.h"

inline float	pointDist(unsigned int dx, unsigned int dy)
{
  return (std::sqrt(dx * dx + dy * dy));
}
