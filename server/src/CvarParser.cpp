#include <fstream>
#include <sstream>
#include <iterator>

#include "CvarParser.hpp"
#include "Exception.hpp"
#include "StringUtils.hpp"

CvarParser::CvarParser(ACvar &cvars) :
  _cvars(cvars)
{
  _parseKey["set"] = &CvarParser::setKeyword;
  _parseKey["exec"] = &CvarParser::execKeyword;
}

CvarParser::~CvarParser()
{
}

void	CvarParser::setKeyword(const std::vector<std::string> &tokens)
{
  if (tokens.size() < 3)
    throw (Exception("Missing parameters for set command"));
  _cvars.setCvar(tokens[1], tokens[2]);
}

void	CvarParser::execKeyword(const std::vector<std::string> &tokens)
{
  if (tokens.size() < 2)
    throw (Exception("Missing parameters for exec command"));
  loadConfigFile(tokens[1]);
}

void	CvarParser::parseCommandLine(const std::string &cmd)
{
  std::vector<std::string>	tokens;
  StringUtils	su;

  su.tokenString(cmd, tokens);
  if (tokens.empty())
    return ;
  else
    {
      auto elem = _parseKey.find(tokens[0]);
      if (elem != _parseKey.end())
	(this->*(elem->second))(tokens);
      else
	throw (Exception("Invalid Keyword ["+ *tokens.begin() + "]"));
    }
}

void	CvarParser::loadConfigFile(const std::string &filename)
{
  std::ifstream	file(filename.c_str());
  std::string	line;
  std::vector<std::string>	content;

  if (file.is_open())
    {
      while (std::getline(file, line))
	content.push_back(line);
      file.close();
    }
  else
    throw (Exception("File [" + filename + "] not found""]"));
  for (const auto &command : content)
    parseCommandLine(command);
}
