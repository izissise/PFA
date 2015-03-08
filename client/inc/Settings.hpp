#ifndef _SETTINGS_H_
# define _SETTINGS_H_

#include <string>
#include "CvarList.hpp"
#include "Controls.hpp"

class Console;

class Settings
{
public:
  Settings();
  virtual ~Settings();

  CvarList	&getCvarList();
  Controls	&getControls();

private:
  CvarList			_vars;
  Controls			_ctrl;
};

#endif /* _SETTINGS_H_ */
