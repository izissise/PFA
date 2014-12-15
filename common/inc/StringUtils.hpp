#ifndef _STRINGUTILS_H_
#define _STRINGUTILS_H_

#include <string>
#include <vector>
#include <sstream>

class StringUtils
{
public:
  StringUtils();
  virtual ~StringUtils();

  void		split(const std::string &s, char delim,
		      std::vector<std::string> &elems);
};

#endif /* _STRINGUTILS_H_ */
