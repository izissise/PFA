#include <cstdlib>
#include <sstream>
#include "GuidGenerator.hpp"
#include "printv.hpp"

GuidGenerator::GuidGenerator()
{
}

GuidGenerator::~GuidGenerator()
{
}

#ifdef _WIN32
# include <intrin.h>

unsigned long long int	GuidGenerator::rdtsc()
{
  return __rdtsc();
}

#else

unsigned long long int	GuidGenerator::rdtsc()
{
  unsigned int lo;
  unsigned int hi;

  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return (static_cast<unsigned long long int>(hi) << 32) | lo;
}

#endif

std::string		GuidGenerator::generate()
{
  std::stringstream	ss("");
  std::ostringstream	tmp;
  unsigned int		randNb;
  const std::string	table("89ab");

  for (unsigned int idx = 0; idx < 16; ++idx)
    {
      randNb = rdtsc() % 255;
      if (idx == 6)
	ss << "4" << std::hex << (randNb % 15);
      else if (idx == 8)
	{
	  ss << table[rdtsc() % table.size()];
	  ss << std::hex << (randNb % 15);
	}
      else
	{
	  if (randNb <= 0xF)
	    ss << "0";
	  ss << std::hex << randNb;
	}
      for (unsigned int nb : {3,5,7,9})
	{
	  if (nb == idx)
	    ss << "-";
	}
    }
  return ss.str();
}
