#include "CvarList.hpp"

CvarList::CvarList()
{
  addCvar("r_width", new t_cvar({"600", "1", "1920"}, "1600", Number));
  addCvar("r_height", new t_cvar({"800", "1", "1080"}, "900", Number));
  addCvar("com_displayFps", new t_cvar({"60", "1", "1000"}, "60", Number));
  addCvar("com_gameFps", new t_cvar({"125", "20", "300"}, "125", Number));
}
