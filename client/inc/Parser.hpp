#ifndef _PARSER_H_
#define _PARSER_H_

#include "Settings.hpp"

class Parser
{
public:
  Parser(Settings *set);
  virtual ~Parser();

  const std::string	&loadConfigFile(const std::string &filename);
  const std::string	&parseLine(const std::string &cmd, bool isFile = true);

private:
  typedef	void (Parser::*func)(const std::vector<std::string> &vec);
  void		parseCommandLine(const std::string &cmd, bool isFile);
  void		setKeyword(const std::vector<std::string> &tokens);
  void		bindKeyword(const std::vector<std::string> &tokens);
  void		unbindKeyword(const std::vector<std::string> &tokens);
  void		split(const std::string &s, char delim,
		      std::vector<std::string> &elems);

private:
  std::map<std::string, func>	_parseKey;
  std::string			_retVal;
  Settings			*_set;
};


#endif /* _PARSER_H_ */
