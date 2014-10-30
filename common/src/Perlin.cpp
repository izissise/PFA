#include <math.h>
#include "Perlin.h"

float	perlin_grad2d(int hash, float x, float y)
{
  int	conv_hash = hash & 15;
  float	u = conv_hash < 8 ? x : y;
  float	v = conv_hash < 4 ? y : (conv_hash == 12 || conv_hash == 14) ? x : 0;
  return ((conv_hash & 1) == 0 ? u : -u) + ((conv_hash & 2) == 0 ? v : -v);
}

float	perlin2d(float x, float y)
{
  int	X = ((int)floorf(x)) & 255;
  int	Y = ((int)floorf(y)) & 255;

  x -= floorf(x);
  y -= floorf(y);
  float u = FADE(x);
  float v = FADE(y);

  int	A = perm[X] + Y;
  int	AA = perm[A];
  int	AB = perm[A + 1];
  int	B = perm[X + 1] + Y;
  int	BA = perm[B];
  int	BB = perm[B + 1];

  return LERP(v, LERP(u, perlin_grad2d(perm[AA], x , y),
			perlin_grad2d(perm[BA], x-1, y)),
	          LERP(u, perlin_grad2d(perm[AB], x , y - 1),
			perlin_grad2d(perm[BB] , x - 1, y - 1)));
}
