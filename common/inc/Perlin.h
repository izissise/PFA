#ifndef _PERLIN_H_
#define _PERLIN_H_

#define FADE(t) (t * t * t * (t * (t * 6 - 15) + 10))
//#define FASTFLOOR(x) (((x) > 0) ? ((int)x) : ((int)x - 1))
#define LERP(t, a, b) ((a) + (t) * ((b) - (a)))

#define S_NOISE_GEN 1013
#define X_NOISE_GEN 1619
#define Y_NOISE_GEN 31337
#define Z_NOISE_GEN 6971

namespace noise
{
  inline int	FASTFLOOR(float val)
  {
    return (((val) > 0) ? (int)(val) : (int)((val) - 1));
  }
  float	perlin_grad2d(int hash, float x, float y);
  float	perlin2d(float x, float y);

  void	setSeed(unsigned int value);
  unsigned int getSeed();
  unsigned int getPerm(unsigned int pos);
  void	initPerm();

  extern unsigned char perm[512];
}

#endif /* _PERLIN_H_ */
