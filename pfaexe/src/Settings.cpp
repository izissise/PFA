#include "Settings.hpp"

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
