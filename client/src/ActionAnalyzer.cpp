#include "ActionAnalyzer.hpp"

ActionAnalyzer::ActionAnalyzer() :
  _oldActionState(static_cast<int>(Action::Last), false),
  _diffState(static_cast<int>(Action::Last), -1)
{
}

ActionAnalyzer::~ActionAnalyzer()
{
}

unsigned int	ActionAnalyzer::getInputChanges(Settings &set)
{
  Controls	&ctrl = set.getControls();
  bool		cuState;
  int		idx;
  unsigned int	changes = 0;

  std::fill(_diffState.begin(), _diffState.end(), -1);
  for (Action act = Action::Forward; act < Action::Last; ++act)
    {
      cuState = ctrl.getActionState(act);
      idx = static_cast<int>(act);
      if (cuState != _oldActionState[idx])
	{
	  _oldActionState[idx] = cuState;
	  _diffState[idx] = cuState;
	  ++changes;
	}
      else
	_diffState[idx] = -1;
    }
  return changes;
}

std::string	ActionAnalyzer::serialize()
{
  unsigned int	action = -1;
  std::string	serialized;

  for (auto state : _diffState)
    {
      ++action;
      if (state == -1)
	continue ;
    }
  return serialized;
}
