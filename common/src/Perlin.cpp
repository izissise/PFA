#include <cstdlib>
#include "Perlin.h"

namespace noise
{
  static unsigned int	n_seed = 1234;
  unsigned char		perm[512];
}

using namespace noise;

void	noise::setSeed(unsigned int value)
{
  n_seed = value;
  std::srand(value);
}

unsigned int	noise::getSeed()
{
  return n_seed;
}

unsigned int	noise::getPerm(unsigned int pos)
{
  return perm[pos % (sizeof(perm) / sizeof(unsigned char))];
}

void		noise::initPerm()
{
  for (unsigned int x = 0; x < sizeof(perm) / sizeof(unsigned char); ++x)
    {
      perm[x] = 256.f * ((float)std::rand() / (float)RAND_MAX);
    }
}

