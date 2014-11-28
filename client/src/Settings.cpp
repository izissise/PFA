#include "Settings.hpp"
#include "Exception.hpp"

Settings::Settings()
{
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
