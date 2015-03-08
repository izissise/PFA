#ifndef _CVARPARSER_H_
#define _CVARPARSER_H_

#include <map>
#include <vector>
#include "ACvar.hpp"

class CvarParser
{
public:
  CvarParser(ACvar &cvars);
  virtual ~CvarParser();

  void		loadConfigFile(const std::string &filename);

private:
  typedef	void (CvarParser::*func)(const std::vector<std::string> &vec);
  void		parseCommandLine(const std::string &cmd);
  void		execKeyword(const std::vector<std::string> &tokens);
  void		setKeyword(const std::vector<std::string> &tokens);

private:
  std::map<std::string, func>	_parseKey;
  ACvar				&_cvars;
};

#endif /* _CVARPARSER_H_ */
