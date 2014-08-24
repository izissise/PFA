#include "Controls.hpp"

Controls::Controls()
{
}

Controls::~Controls()
{
}

bool	Controls::isPressed(key k) const
{
  auto	it = _keyState.find(k);

  return ((it == _keyState.end()) ? false : it->second);
}

Controls::Action	Controls::getActionFromKey(key k) const
{
  auto	it = _keyAction.find(k);

  return ((it == _keyAction.end()) ? Unknown : it->second);
}

bool	Controls::getActionState(Action act) const
{
  key	k = -1;

  for (auto &it : _keyAction)
    if (it.second == act)
      {
	k = it.first;
	break ;
      }
  return ((k == -1) ? false : isPressed(k));
}
