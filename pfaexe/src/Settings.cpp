#include <fstream>
#include <sstream>
#include <iterator>
#include "Settings.hpp"
#include "Exception.hpp"

Settings::Settings()
{
  _parseKey["set"] = &Settings::setKeyword;
  _parseKey["bind"] = &Settings::bindKeyword;
  loadConfigFile("../config.cfg");
}

Settings::~Settings()
{
}

CvarList	&Settings::getCvarList()
{
  return (_vars);
}

void	Settings::setKeyword(const std::vector<std::string> &tokens)
{
  if (tokens.size() < 3)
    throw (Exception("Missing parameters for set command"));
  _vars.setCvar(tokens[1], tokens[2]);
}

void	Settings::bindKeyword(const std::vector<std::string> &tokens)
{
  std::cout << "bind Keyword function" << std::endl;
}

void	Settings::parseCommandLine(const std::string &cmd)
{
  std::istringstream			buf(cmd);
  std::istream_iterator<std::string>	beg(buf), end;
  std::vector<std::string>		tokens(beg, end);

  if (tokens.empty())
    return ;
  auto elem = _parseKey.find(*tokens.begin());
  if (elem == _parseKey.end())
    throw (Exception("Invalid Keyword ["+ *tokens.begin() + "]"));
  (this->*(elem->second))(tokens);
}

void	Settings::loadConfigFile(const std::string &filename)
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
    throw (Exception("File [" + filename + "] not found"));
  for (const auto &it : content)
    try {
      parseCommandLine(it);
    }
    catch (const Exception &e) {
      std::cerr << e.what() << std::endl;
    }
}
