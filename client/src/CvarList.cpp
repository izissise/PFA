#include "CvarList.hpp"

CvarList::CvarList()
{
  addCvar("r_width", new t_cvar({"1", "1920"}, "600", "1600", Number));
  addCvar("r_height", new t_cvar({"1", "1080"}, "800", "900", Number));
  addCvar("com_displayFps", new t_cvar({"1", "1000"}, "60", "60", Number));
  addCvar("com_gameFps", new t_cvar({"20", "300"}, "125", "125", Number));
  addCvar("net_upBandwidth", new t_cvar({"0", "2147483647"}, "60000", "60000", Number));
  addCvar("net_downBandwidth", new t_cvar({"0", "2147483647"}, "1000000", "1000000", Number));
  addCvar("sv_masterIP", new t_cvar({"127.0.0.1"/*"62.210.136.56"*/}, "127.0.0.1",
				    "127.0.0.1", String, Restriction::Value));
  addCvar("sv_masterPort", new t_cvar({"25255"}, "25255",
				      "25255", String, Restriction::Value));
}
