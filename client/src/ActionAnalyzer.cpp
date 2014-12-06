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

std::string		ActionAnalyzer::serialize()
{
  ProtocolMessage	msg;
  ClientActions		*actions = new ClientActions;
  ClientAction		*action;
  std::string		serialized;
  unsigned int		actionId = -1;

  for (auto state : _diffState)
    {
      ++actionId;
      if (state == -1)
	continue ;
      action = actions->add_actions();
      if (action == nullptr)
	continue;
      action->set_name(actionId);
      action->set_state(static_cast<bool>(state));
    }
  msg.set_content(ProtocolMessage::ACTION);
  msg.set_allocated_actions(actions);
  msg.SerializeToString(&serialized);
  return serialized;
}
