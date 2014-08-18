#ifndef _SETTINGS_H_
#define _SETTINGS_H_

# include "Cvar.hpp"

class Settings
{
public:
  Settings();
  virtual ~Settings();

  CvarList	&getCvarList();
private:
  CvarList	_vars;
};

#endif /* _SETTINGS_H_ */
