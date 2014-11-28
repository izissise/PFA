#include "FastMath.h"

unsigned int	approx_distance(int dx, int dy)
{
  unsigned int min;
  unsigned int max;
  unsigned int appDist;

  if (dx < 0)
    dx = std::abs(dx);
  if (dy < 0)
    dy = std::abs(dy);
  if (dx < dy)
    {
      min = dx;
      max = dy;
    }
  else
    {
      min = dy;
      max = dx;
    }
  appDist = (max * 1007) + (min * 441);
  if (max < (min << 4))
    appDist -= (max * 40);
  return ((appDist + 512) >> 10);
}
