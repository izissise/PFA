#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#include <SFML/Window.hpp>
#include <map>
#include <array>
#include <vector>

#include "ActionEnum.hpp"

namespace ctrl
{
  typedef int	key;

  enum class	type
  {
    Keyboard = 0,
      Mouse,
      MouseWheel,
      TypeCount
      };
  enum class	state
  {
    Unset = 0
      };
}

inline Action	operator++(Action &x)
{
  return (x = static_cast<Action>(static_cast<int>(x) + 1));
}

enum class	actionType
{
  Default = 0,
    Toggle
    };

typedef struct	s_entry	t_entry;

struct		s_entry
{
  ctrl::key	key;
  ctrl::type	type;
  s_entry(ctrl::key k = sf::Keyboard::Unknown,
	  ctrl::type t = ctrl::type::Keyboard) :
    key(k), type(t)
  {
  }
  bool	operator==(const t_entry &e) const
  {
    return (key == e.key && type == e.type);
  }
  bool	operator==(ctrl::state e) const
  {
    if (e == ctrl::state::Unset)
      return (key == sf::Keyboard::Unknown && type != ctrl::type::MouseWheel);
    return false;
  }
  void	operator=(t_entry e)
  {
    key = e.key;
    type = e.type;
  }
  void	operator=(ctrl::state e)
  {
    if (e == ctrl::state::Unset)
      {
	key = sf::Keyboard::Unknown;
	type = ctrl::type::Keyboard;
      }
  }
  void	fill(const sf::Event &event)
  {
    if (event.type == sf::Event::KeyPressed ||
	event.type == sf::Event::KeyReleased)
      {
	key = event.key.code;
	type = ctrl::type::Keyboard;
      }
    else if (event.type == sf::Event::MouseButtonPressed ||
	     event.type == sf::Event::MouseButtonReleased)
      {
	key = event.mouseButton.button;
	type = ctrl::type::Mouse;
      }
    else if (event.type == sf::Event::MouseWheelMoved)
      {
	key = event.mouseWheel.delta;
	type = ctrl::type::MouseWheel;
      }
  };
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

  bool		isPressed(const t_entry &entry);
  Action	getActionFromKey(const t_entry &entry) const;
  Action	getActionFromCode(const std::string &code) const;
  bool		getActionState(Action act) const;
  t_entry		getKeyFromCode(const std::string &code) const;
  t_entry		getKeyFromAction(Action act) const;
  t_entry		getLastKey(Action act) const;
  bool			isKnownKey(const t_entry &entry) const;
  const std::string	&getCodeFromKey(const t_entry &entry) const;
  const std::string	&getCodeFromAction(Action act) const;
  const std::array<t_entry, 5>	&getBoundKeys(Action act) const;

  void		unbindKey(const t_entry &entry);
  void		unbindKeyFromAction(const t_entry &entry, Action act);
  void		bindKeyOnAction(const t_entry &entry, Action act);
  void		pressKey(const t_entry &entry);
  void		releaseKey(const t_entry &entry);

private:
  std::array<std::map<ctrl::key, bool>,
	     static_cast<size_t>(ctrl::type::TypeCount)>	_keyState;
  std::map<Action, std::array<t_entry, 5>>		_actionKeys;
  std::map<std::string, t_entry>			_keycode;
  std::vector<t_action>					_actions;
};

#endif /* _CONTROLS_H_ */
