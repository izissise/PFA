#ifndef _SIMPLEXNOISE_H_
#define _SIMPLEXNOISE_H_

#include "Perlin.h"
#define F2 0.366025403 // F2 = 0.5*(sqrt(3.0)-1.0)
#define G2 0.211324865 // G2 = (3.0-Math.sqrt(3.0))/6.0

namespace noise
{
  float fbm_2d(int octaves,
	       float lacunarity,
	       float gain,
	       float scale,
	       float x,
	       float y);

  float fbm_3d(int octaves,
	       float lacunarity,
	       float gain,
	       float scale,
	       float x,
	       float y,
	       float z);

  float scaled_fbm_2d(int octaves,
		      float lacunarity,
		      float gain,
		      float scale,
		      float loBound,
		      float hiBound,
		      float x,
		      float y);

  float scaled_fbm_3d(int octaves,
		      float lacunarity,
		      float gain,
		      float scale,
		      float loBound,
		      float hiBound,
		      float x,
		      float y,
		      float z);

  float scaled_raw_noise_2d(float loBound,
			    float hiBound,
			    float x,
			    float y);

  float scaled_raw_noise_3d(float loBound,
			    float hiBound,
			    float x,
			    float y,
			    float z);

  float	turbulence_2d(int octaves, float lacunarity,
		      float gain, float scale,
		      float x, float y);

  float	turbulence_3d(int octaves, float lacunarity,
		      float gain, float scale,
		      float x, float y, float z);

  float	ridge(float h, float offset);
  float	ridged_mf(int octaves, float lacunarity,
		  float gain, float scale,
		  float offset,
		  int x, int y);

  float	ridged_mf3(int octaves, float lacunarity,
		   float gain, float scale,
		   float offset,
		   int x, int y, int z);

  float raw_noise_2d(float x, float y);
  float raw_noise_3d(float x, float y, float z);

  static inline float dot2(const int* g, float x, float y)
  {
    return g[0]*x + g[1]*y;
  }

  static inline float dot3(const int* g, float x, float y, float z)
  {
    return g[0]*x + g[1]*y + g[2]*z;
  }

  static const int grad3[12][3] = {
    {1,1,0}, {-1,1,0}, {1,-1,0}, {-1,-1,0},
    {1,0,1}, {-1,0,1}, {1,0,-1}, {-1,0,-1},
    {0,1,1}, {0,-1,1}, {0,1,-1}, {0,-1,-1}
  };

  static const int grad4[32][4]= {
    {0,1,1,1},  {0,1,1,-1},  {0,1,-1,1},  {0,1,-1,-1},
    {0,-1,1,1}, {0,-1,1,-1}, {0,-1,-1,1}, {0,-1,-1,-1},
    {1,0,1,1},  {1,0,1,-1},  {1,0,-1,1},  {1,0,-1,-1},
    {-1,0,1,1}, {-1,0,1,-1}, {-1,0,-1,1}, {-1,0,-1,-1},
    {1,1,0,1},  {1,1,0,-1},  {1,-1,0,1},  {1,-1,0,-1},
    {-1,1,0,1}, {-1,1,0,-1}, {-1,-1,0,1}, {-1,-1,0,-1},
    {1,1,1,0},  {1,1,-1,0},  {1,-1,1,0},  {1,-1,-1,0},
    {-1,1,1,0}, {-1,1,-1,0}, {-1,-1,1,0}, {-1,-1,-1,0}
  };

  static const unsigned char simplex[64][4] = {
    {0,1,2,3},{0,1,3,2},{0,0,0,0},{0,2,3,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,2,3,0},
    {0,2,1,3},{0,0,0,0},{0,3,1,2},{0,3,2,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,3,2,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {1,2,0,3},{0,0,0,0},{1,3,0,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,3,0,1},{2,3,1,0},
    {1,0,2,3},{1,0,3,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,3,1},{0,0,0,0},{2,1,3,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {2,0,1,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,0,1,2},{3,0,2,1},{0,0,0,0},{3,1,2,0},
    {2,1,0,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,1,0,2},{0,0,0,0},{3,2,0,1},{3,2,1,0}
  };
}

#endif /* _SIMPLEXNOISE_H_ */