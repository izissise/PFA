#ifndef _COHERENTNOISE_H_
#define _COHERENTNOISE_H_

#include "Perlin.h"
#include "Interp.h"

namespace noise
{
  unsigned int intNoise1d(int x, unsigned int seed);
  float	valueNoise1d(int x, unsigned int seed);
  float	valueNoise2d(int x, int y, unsigned int seed);
  float	valueNoise3d(int x, int y, int z, unsigned int seed);
  float	valueCoherentNoise1d(float x,
			     float scale,
			     unsigned int seed,
			     NoiseQuality quality = NoiseQuality::Low);
  float	valueCoherentNoise2d(float x, float y,
			     float scale,
			     unsigned int seed,
			     NoiseQuality quality = NoiseQuality::Low);
  float	valueCoherentNoise3d(float x, float y, float z,
			     float scale,
			     unsigned int seed,
			     NoiseQuality quality = NoiseQuality::Low);
}

#endif /* _COHERENTNOISE_H_ */
