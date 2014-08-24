#ifndef _CONTROLS_H_
#define _CONTROLS_H_

# include <map>

typedef unsigned int	key;

class Controls
{
public:
  enum	Action
    {
      Unknown = -1,
      Forward = 0,
      Back,
      Right,
      Left,
      Use,
      Last
    };

public:
  Controls();
  virtual ~Controls();

  bool		isPressed(key k) const;
  Action	getActionFromKey(key k) const;
  bool		getActionState(Action act) const;
private:
  std::map<key, bool>		_keyState;
  std::map<key, Action>		_keyAction;
};

#endif /* _CONTROLS_H_ */
