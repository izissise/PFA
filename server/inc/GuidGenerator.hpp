#ifndef _GUIDGENERATOR_H_
#define _GUIDGENERATOR_H_

#include <string>

class GuidGenerator
{
public:
  GuidGenerator();
  virtual ~GuidGenerator();

  std::string	generate();

private:
  void		generateUuid4(std::stringstream &ss);
  void		formatCpuCycle(std::stringstream &ss);

private:
  unsigned long long int	rdtsc();
};

#endif /* _GUIDGENERATOR_H_ */
