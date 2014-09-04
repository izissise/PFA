#ifndef _CONTROLS_H_
#define _CONTROLS_H_

# include <SFML/Window.hpp>
# include <map>
# include <array>
# include <vector>

typedef int	key;

enum class	Action
{
  Unknown = -1,
    Forward = 0,
    Back,
    Right,
    Left,
    Use,
    ToggleConsole,
    Last
    };

enum class	actionType
{
  Default = 0,
    Toggle
    };

typedef struct	s_action
{
  std::string	code;
  actionType	type;
  bool		state;

  s_action(const std::string &c,
	       actionType t = actionType::Default) :
    code(c), type(t), state(false)
  {
  }
}		t_action;

class Controls
{
public:
  Controls();
  virtual ~Controls();

  bool		isPressed(key k) const;
  Action	getActionFromKey(key k) const;
  Action	getActionFromCode(const std::string &code) const;
  bool		getActionState(Action act) const;
  key		getKeyFromCode(const std::string &code) const;
  const std::string	&getCodeFromKey(key k) const;
  const std::string	&getCodeFromAction(Action act) const;

  void		bindActionOnKey(key k, Action act);
  void		pressKey(key k);
  void		releaseKey(key k);

private:
  std::map<key, bool>			_keyState;
  std::map<Action, std::array<key, 5>>	_actionKeys;
  std::map<std::string, key>		_keycode;
  std::vector<t_action>			_actions;
};

#endif /* _CONTROLS_H_ */
