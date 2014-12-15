#include <cstdlib>
#include <sstream>
#include <climits>
#include <iomanip>
#include <chrono>
#include "GuidGenerator.hpp"
#include "printv.hpp"

using namespace std::chrono;

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

void			GuidGenerator::formatCpuCycle(std::stringstream &ss)
{
  std::ostringstream	oss;
  unsigned int		ullSize;
  unsigned long long int ullNum;
  std::string		number;
  unsigned int		idx;

  ullNum = rdtsc();
  oss << ULLONG_MAX;
  ullSize = oss.str().size();
  oss.str(std::string());
  oss.clear();
  oss << std::setfill('0') << std::setw(ullSize);
  oss << ullNum;
  number = oss.str();
  for (idx = 0; number.at(idx) == '0' && idx < number.size(); ++idx)
    number.at(idx) = ('a' + std::rand() % (('z' - 'a') + 1)); // 26 :D
  ss << "-" << number.substr(0, idx) << "-" << number.substr(idx);
}

void			GuidGenerator::generateUuid4(std::stringstream &ss)
{
  const std::string	table("89ab");
  unsigned int		randNb;

  for (unsigned int idx = 0; idx < 16; ++idx)
    {
      randNb = std::rand() % 255;
      if (idx == 6)
	ss << "4" << std::hex << (randNb % 15);
      else if (idx == 8)
	{
	  ss << table[std::rand() % table.size()];
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
}

std::string		GuidGenerator::generate()
{
  std::stringstream	ss("");

  generateUuid4(ss);
  formatCpuCycle(ss);
  return ss.str();
}
