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
  unsigned long long int	rdtsc();
};

#endif /* _GUIDGENERATOR_H_ */
