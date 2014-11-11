#include <fstream>
#include <sstream>
#include <iterator>
#include "Settings.hpp"
#include "Exception.hpp"

Settings::Settings()
{
  _parseKey["set"] = &Settings::setKeyword;
  _parseKey["bind"] = &Settings::bindKeyword;
  _parseKey["unbind"] = &Settings::unbindKeyword;
  loadConfigFile("../config.cfg");
}

Settings::~Settings()
{
}

CvarList	&Settings::getCvarList()
{
  return (_vars);
}

Controls	&Settings::getControls()
{
  return (_ctrl);
}

void	Settings::setKeyword(const std::vector<std::string> &tokens)
{
  if (tokens.size() < 3)
    throw (Exception("Missing parameters for set command"));
  _vars.setCvar(tokens[1], tokens[2]);
}

void		Settings::bindKeyword(const std::vector<std::string> &tokens)
{
  t_entry	entry;
  Action	act;

  if (tokens.size() < 3)
    throw (Exception("Missing parameters for bind command"));
  entry = _ctrl.getKeyFromCode(tokens[1]);
  // MouseWheelDown is -1 same as sf::Keyboard::Unknown
  if (entry == ctrl::state::Unset)
    throw (Exception("Unknown key ["+ tokens[1] +"]"));
  act = _ctrl.getActionFromCode(tokens[2]);
  if (act == Action::Unknown)
    throw (Exception("Unknown Action ["+ tokens[2] +"] for key ["+ tokens[1] +"]"));
  _ctrl.bindKeyOnAction(entry, act);
}

void		Settings::unbindKeyword(const std::vector<std::string> &tokens)
{
  t_entry	entry;
  Action	act;

  if (tokens.size() < 2)
    throw (Exception("Missing parameters for bind command"));
  entry = _ctrl.getKeyFromCode(tokens[1]);
  if (entry == ctrl::state::Unset)
    throw (Exception("Unknown key ["+ tokens[1] +"]"));
  act = _ctrl.getActionFromKey(entry);
  if (act == Action::Unknown)
    throw (Exception("Key ["+ tokens[1] +"] is not bound"));
  _ctrl.unbindKeyFromAction(entry, act);
}

void	Settings::parseCommandLine(const std::string &cmd, bool isFile)
{
  std::istringstream			buf(cmd);
  std::istream_iterator<std::string>	beg(buf), end;
  std::vector<std::string>		tokens(beg, end);

  if (tokens.empty())
    return ;
  else if (tokens[0].at(0) == '/' || isFile == true)
    {
      if (tokens[0].at(0) == '/')
	tokens[0] = tokens[0].substr(1);
      auto elem = _parseKey.find(*tokens.begin());
      if (elem == _parseKey.end())
	throw (Exception("Invalid Keyword ["+ *tokens.begin() + "]"));
      (this->*(elem->second))(tokens);
    }
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
  {
    try
      {
	parseCommandLine(it, true);
      }
    catch (const Exception &e)
      {
	std::cerr << e.what() << std::endl;
      }
  }
}
