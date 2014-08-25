#ifndef _CONTROLS_H_
#define _CONTROLS_H_

# include <SFML/Window.hpp>
# include <map>
# include <vector>

typedef unsigned int	key;

enum class	Action
  {
    Unknown = -1,
    Forward = 0,
    Back,
    Right,
    Left,
    Use,
    Last
  };

class Controls
{
private:
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
private:
  std::map<key, bool>		_keyState;
  std::map<key, Action>		_keyAction;
  std::map<std::string, key>	_keycode;
  std::vector<std::string>	_actions;
};

#endif /* _CONTROLS_H_ */
