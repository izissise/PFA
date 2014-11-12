#include <fstream>
#include <sstream>
#include <iterator>
#include "Parser.hpp"
#include "Exception.hpp"

Parser::Parser(Settings *set) :
  _set(set)
{
  _parseKey["set"] = &Parser::setKeyword;
  _parseKey["bind"] = &Parser::bindKeyword;
  _parseKey["unbind"] = &Parser::unbindKeyword;
  _parseKey["include"] = &Parser::execKeyword;
}

Parser::~Parser()
{
}

void		Parser::setKeyword(const std::vector<std::string> &tokens)
{
  CvarList	&vars = _set->getCvarList();

  if (tokens.size() < 3)
    throw (Exception("Missing parameters for set command"));
  vars.setCvar(tokens[1], tokens[2]);
}

void		Parser::bindKeyword(const std::vector<std::string> &tokens)
{
  Controls	&ctrl = _set->getControls();
  t_entry	entry;
  Action	act;

  if (tokens.size() < 3)
    throw (Exception("Missing parameters for bind command"));
  entry = ctrl.getKeyFromCode(tokens[1]);
  // MouseWheelDown is -1 same as sf::Keyboard::Unknown
  if (entry == ctrl::state::Unset)
    throw (Exception("Unknown key ["+ tokens[1] +"]"));
  act = ctrl.getActionFromCode(tokens[2]);
  if (act == Action::Unknown)
    throw (Exception("Unknown Action ["+ tokens[2] +"] for key ["+ tokens[1] +"]"));
  ctrl.bindKeyOnAction(entry, act);
}

void		Parser::unbindKeyword(const std::vector<std::string> &tokens)
{
  Controls	&ctrl = _set->getControls();
  t_entry	entry;
  Action	act;

  if (tokens.size() < 2)
    throw (Exception("Missing parameters for bind command"));
  entry = ctrl.getKeyFromCode(tokens[1]);
  if (entry == ctrl::state::Unset)
    throw (Exception("Unknown key ["+ tokens[1] +"]"));
  act = ctrl.getActionFromKey(entry);
  if (act == Action::Unknown)
    throw (Exception("Key ["+ tokens[1] +"] is not bound"));
  ctrl.unbindKeyFromAction(entry, act);
}

void	Parser::execKeyword(const std::vector<std::string> &tokens)
{
  if (tokens.size() < 2)
    throw (Exception("Missing parameters for exec command"));
  loadConfigFile(tokens[1]);
}

void	Parser::parseCommandLine(const std::string &cmd, bool isFile)
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

void	Parser::split(const std::string &s, char delim,
		      std::vector<std::string> &elems)
{
  std::stringstream	ss(s);
  std::string		item;

  while (std::getline(ss, item, delim))
    {
      if (!item.empty())
	elems.push_back(item);
    }
}

const std::string	&Parser::parseLine(const std::string &cmd, bool isFile)
{
  std::vector<std::string> elems;

  _retVal.clear();
  split(cmd, ';', elems);
  for (auto &command : elems)
    {
      try
	{
	  parseCommandLine(command, isFile);
	}
      catch (const Exception &e)
	{
	  _retVal = e.what();
	}
    }
  return _retVal;
}

const std::string	&Parser::loadConfigFile(const std::string &filename)
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
      if (!parseLine(it, true).empty())
	return _retVal;
    }
  return _retVal;
}
