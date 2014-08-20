#ifndef _SETTINGS_H_
#define _SETTINGS_H_

# include <map>
# include <vector>
# include <string>
# include "Cvar.hpp"

class Settings
{
public:
  Settings();
  virtual ~Settings();

  CvarList	&getCvarList();
  void		loadConfigFile(const std::string &filename);
  void		parseCommandLine(const std::string &cmd);

private:
  typedef	void (Settings::*func)(const std::vector<std::string> &vec);
  void		setKeyword(const std::vector<std::string> &tokens);
  void		bindKeyword(const std::vector<std::string> &tokens);

  CvarList			_vars;
  std::map<std::string, func>	_parseKey;
};

#endif /* _SETTINGS_H_ */