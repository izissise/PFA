#ifndef _PARSER_H_
#define _PARSER_H_

#include "Settings.hpp"

class Parser
{
public:
  Parser(Settings *set);
  virtual ~Parser();

  const std::vector<std::string>	&loadConfigFile(const std::string &filename);
  const std::vector<std::string>	&parseLine(const std::string &cmd, bool isFile = false);

private:
  typedef	void (Parser::*func)(const std::vector<std::string> &vec);
  void		parseCommandLine(const std::string &cmd, bool isFile);
  void		setKeyword(const std::vector<std::string> &tokens);
  void		bindKeyword(const std::vector<std::string> &tokens);
  void		unbindKeyword(const std::vector<std::string> &tokens);
  void		execKeyword(const std::vector<std::string> &tokens);
  void		giveCvarInfo(const std::vector<std::string> &tokens);
  void		split(const std::string &s, char delim,
		      std::vector<std::string> &elems);

private:
  std::map<std::string, func>	_parseKey;
  std::vector<std::string>	_retVal;
  Settings			*_set;
};


#endif /* _PARSER_H_ */
