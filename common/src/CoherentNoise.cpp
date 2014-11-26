#include <iostream>
#include "SimplexNoise.h"
#include "CoherentNoise.h"

using namespace noise;

unsigned int	noise::intNoise1d(int x, unsigned int seed)
{
  long	n = (
	     X_NOISE_GEN * x
	     + S_NOISE_GEN * seed
	     ) & 0x7fffffff;
  n = (n >> 13) ^ n;
  n = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
  return n;
}

float	noise::valueNoise1d(int x, unsigned int seed)
{
  long	n = (
	     X_NOISE_GEN * x
	     + S_NOISE_GEN * seed
	     ) & 0x7fffffff;
  n = (n >> 13) ^ n;
  n = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
  return 1.0 - ((double)n / 1073741824.0);
}

float	noise::valueNoise2d(int x, int y, unsigned int seed)
{
  long n = (
	    X_NOISE_GEN * x
	    + Y_NOISE_GEN  * y
	    + S_NOISE_GEN * seed
	    ) & 0x7fffffff;
  n = (n >> 13) ^ n;
  n = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
  return 1.0 - ((double)n / 1073741824.0);
}

float	noise::valueNoise3d(int x, int y, int z,
			    unsigned int seed)
{
  long n = (
	    X_NOISE_GEN * x
	    + Y_NOISE_GEN  * y
	    + Z_NOISE_GEN * z
	    + S_NOISE_GEN * seed
	    ) & 0x7fffffff;
  n = (n >> 13) ^ n;
  n = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
  return 1.0 - ((double)n / 1073741824.0);
}

float	noise::valueCoherentNoise1d(float x, float scale,
				    unsigned int seed,
				    NoiseQuality quality)
{
  x *= scale;
  int	X0 = FASTFLOOR(x);
  float IX = interp(x, X0, quality);

  float n0 = valueNoise1d(X0, seed);
  float n1 = valueNoise1d(X0 + 1, seed);
  return linear(n0, n1, IX);
}

float	noise::valueCoherentNoise2d(float x, float y, float scale,
				    unsigned int seed, NoiseQuality quality)
{
  x *= scale;
  y *= scale;
  int	X0 = FASTFLOOR(x);
  int	Y0 = FASTFLOOR(y);
  int	X1 = X0 + 1;
  int	Y1 = Y0 + 1;
  float IX = interp(x, X0, quality);
  float IY = interp(y, Y0, quality);

  float n0 = valueNoise2d(X0, Y0, seed);
  float n1 = valueNoise2d(X1, Y0, seed);
  float IN0 = linear(n0, n1, IX);
  n0 = valueNoise2d(X0, Y1, seed);
  n1 = valueNoise2d(X1, Y1, seed);
  float IN1 = linear(n0, n1, IY);
  return linear(IN0, IN1, IY);
}

float	noise::valueCoherentNoise3d(float x, float y, float z, float scale,
				    unsigned int seed, NoiseQuality quality)
{
  x *= scale;
  y *= scale;
  z *= scale;
  int	X0 = FASTFLOOR(x);
  int	Y0 = FASTFLOOR(y);
  int	Z0 = FASTFLOOR(z);
  int	X1 = X0 + 1;
  int	Y1 = Y0 + 1;
  int	Z1 = Z0 + 1;
  float IX = interp(x, X0, quality);
  float IY = interp(y, Y0, quality);
  float IZ = interp(z, Z0, quality);

  float n0 = valueNoise3d(X0, Y0, Z0, seed);
  float n1 = valueNoise3d(X1, Y0, Z0, seed);
  float IX0 = linear(n0, n1, IX);

  n0 = valueNoise3d(X0, Y1, Z0, seed);
  n1 = valueNoise3d(X1, Y1, Z0, seed);
  float IX1 = linear(n0, n1, IX);
  float IY0 = linear(IX0, IX1, IY);

  n0 = valueNoise3d(X0, Y0, Z1, seed);
  n1 = valueNoise3d(X1, Y0, Z1, seed);
  IX0 = linear(n0, n1, IX);

  n0 = valueNoise3d(X0, Y1, Z1, seed);
  n1 = valueNoise3d(X1, Y1, Z1, seed);
  IX1 = linear(n0, n1, IX);
  float IY1 = linear(IX0, IX1, IY);
  return linear(IY0, IY1, IZ);
}
