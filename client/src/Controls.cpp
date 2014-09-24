#include <algorithm>
#include "Controls.hpp"
#include "Exception.hpp"

Controls::Controls()
{
  using namespace sf;

  _keycode["A"] = Keyboard::A;
  _keycode["B"] = Keyboard::B;
  _keycode["C"] = Keyboard::C;
  _keycode["D"] = Keyboard::D;
  _keycode["E"] = Keyboard::E;
  _keycode["F"] = Keyboard::F;
  _keycode["G"] = Keyboard::G;
  _keycode["H"] = Keyboard::H;
  _keycode["I"] = Keyboard::I;
  _keycode["J"] = Keyboard::J;
  _keycode["K"] = Keyboard::K;
  _keycode["L"] = Keyboard::L;
  _keycode["M"] = Keyboard::M;
  _keycode["N"] = Keyboard::N;
  _keycode["O"] = Keyboard::O;
  _keycode["P"] = Keyboard::P;
  _keycode["Q"] = Keyboard::Q;
  _keycode["R"] = Keyboard::R;
  _keycode["S"] = Keyboard::S;
  _keycode["T"] = Keyboard::T;
  _keycode["U"] = Keyboard::U;
  _keycode["V"] = Keyboard::V;
  _keycode["W"] = Keyboard::W;
  _keycode["X"] = Keyboard::X;
  _keycode["Y"] = Keyboard::Y;
  _keycode["Z"] = Keyboard::Z;
  _keycode["NUM0"] = Keyboard::Num0;
  _keycode["NUM1"] = Keyboard::Num1;
  _keycode["NUM2"] = Keyboard::Num2;
  _keycode["NUM3"] = Keyboard::Num3;
  _keycode["NUM4"] = Keyboard::Num4;
  _keycode["NUM5"] = Keyboard::Num5;
  _keycode["NUM6"] = Keyboard::Num6;
  _keycode["NUM7"] = Keyboard::Num7;
  _keycode["NUM8"] = Keyboard::Num8;
  _keycode["NUM9"] = Keyboard::Num9;
  _keycode["ESCAPE"] = Keyboard::Escape;
  _keycode["L_CTRL"] = Keyboard::LControl;
  _keycode["L_SHIFT"] = Keyboard::LShift;
  _keycode["L_ALT"] = Keyboard::LAlt;
  _keycode["L_SYSTEM"] = Keyboard::LSystem;
  _keycode["R_CTRL"] = Keyboard::RControl;
  _keycode["R_SHIFT"] = Keyboard::RShift;
  _keycode["R_ALT"] = Keyboard::RAlt;
  _keycode["R_SYSTEM"] = Keyboard::RSystem;
  _keycode["MENU"] = Keyboard::Menu;
  _keycode["LBRACKET"] = Keyboard::LBracket;
  _keycode["RBRACKET"] = Keyboard::RBracket;
  _keycode["SEMICOLON"] = Keyboard::SemiColon;
  _keycode["COMMA"] = Keyboard::Comma;
  _keycode["PERIOD"] = Keyboard::Period;
  _keycode["SLASH"] = Keyboard::Slash;
  _keycode["BACKSLASH"] = Keyboard::BackSlash;
  _keycode["Tilde"] = Keyboard::Tilde;
  _keycode["EQUAL"] = Keyboard::Equal;
  _keycode["DASH"] = Keyboard::Dash;
  _keycode["SPACE"] = Keyboard::Space;
  _keycode["ENTER"] = Keyboard::Return;
  _keycode["BACKSPACE"] = Keyboard::BackSpace;
  _keycode["TAB"] = Keyboard::Tab;
  _keycode["PGUP"] = Keyboard::PageUp;
  _keycode["PGDN"] = Keyboard::PageDown;
  _keycode["HOME"] = Keyboard::Home;
  _keycode["INS"] = Keyboard::Insert;
  _keycode["DEL"] = Keyboard::Delete;
  _keycode["PLUS"] = Keyboard::Add;
  _keycode["MINUS"] = Keyboard::Subtract;
  _keycode["MULT"] = Keyboard::Multiply;
  _keycode["DIV"] = Keyboard::Divide;
  _keycode["LEFTARROW"] = Keyboard::Left;
  _keycode["RIGHTARROW"] = Keyboard::Right;
  _keycode["UPARROW"] = Keyboard::Up;
  _keycode["DOWNARROW"] = Keyboard::Down;
  _keycode["KP_0"] = Keyboard::Numpad0;
  _keycode["KP_1"] = Keyboard::Numpad1;
  _keycode["KP_2"] = Keyboard::Numpad2;
  _keycode["KP_3"] = Keyboard::Numpad3;
  _keycode["KP_4"] = Keyboard::Numpad4;
  _keycode["KP_5"] = Keyboard::Numpad5;
  _keycode["KP_6"] = Keyboard::Numpad6;
  _keycode["KP_7"] = Keyboard::Numpad7;
  _keycode["KP_8"] = Keyboard::Numpad8;
  _keycode["KP_9"] = Keyboard::Numpad9;
  _keycode["F1"] = Keyboard::F1;
  _keycode["F2"] = Keyboard::F2;
  _keycode["F3"] = Keyboard::F3;
  _keycode["F4"] = Keyboard::F4;
  _keycode["F5"] = Keyboard::F5;
  _keycode["F6"] = Keyboard::F6;
  _keycode["F7"] = Keyboard::F7;
  _keycode["F8"] = Keyboard::F8;
  _keycode["F9"] = Keyboard::F9;
  _keycode["F10"] = Keyboard::F10;
  _keycode["F11"] = Keyboard::F11;
  _keycode["F12"] = Keyboard::F12;
  _keycode["F13"] = Keyboard::F13;
  _keycode["F14"] = Keyboard::F14;
  _keycode["F15"] = Keyboard::F15;
  _keycode["PAUSE"] = Keyboard::Pause;

  // _keycode["MOUSE1"] = K_MOUSE1;
  // _keycode["MOUSE2"] = K_MOUSE2;
  // _keycode["MOUSE3"] = K_MOUSE3;
  // _keycode["MOUSE4"] = K_MOUSE4;
  // _keycode["MOUSE5"] = K_MOUSE5;
  // _keycode["MWHEELUP"] = K_MWHEELUP;
  // _keycode["MWHEELDOWN"] = K_MWHEELDOWN;
  _actions.push_back(t_action("forward"));
  _actions.push_back(t_action("back"));
  _actions.push_back(t_action("right"));
  _actions.push_back(t_action("left"));
  _actions.push_back(t_action("use"));
  _actions.push_back(t_action("console", actionType::Toggle));
}

Controls::~Controls()
{
}

bool	Controls::isPressed(ctrl::key k) const
{
  auto	it = _keyState.find(k);

  return ((it == _keyState.end()) ? false : it->second);
}

Action	Controls::getActionFromKey(ctrl::key k) const
{
  if (k == sf::Keyboard::Unknown)
    return Action::Unknown;
  for (auto &it : _actionKeys)
    {
      for (auto &elem : it.second)
	if (elem == k)
	  return it.first;
    }
  return Action::Unknown;
}

Action	Controls::getActionFromCode(const std::string &code) const
{
  auto	it = std::find_if(_actions.begin(), _actions.end(), [&code](const t_action &act)
			  { return (act.code == code); });

  return ((it == _actions.end()) ? Action::Unknown :
	  static_cast<Action>(std::distance(_actions.begin(), it)));
}

const std::string	&Controls::getCodeFromAction(Action act) const
{
  if (static_cast<int>(act) < 0 || static_cast<unsigned int>(act) >= _actions.size())
    throw (std::out_of_range("No such action"));
  return (_actions[static_cast<unsigned int>(act)].code);
}

bool	Controls::getActionState(Action act) const
{
  if (static_cast<int>(act) < 0 || static_cast<unsigned int>(act) >= _actions.size())
    throw (std::out_of_range("No such action"));
  return (_actions[static_cast<unsigned int>(act)].state);
}

ctrl::key	Controls::getKeyFromCode(const std::string &code) const
{
  auto	it = _keycode.find(code);

  return ((it == _keycode.end()) ? sf::Keyboard::Unknown : it->second);
}

ctrl::key	Controls::getKeyFromAction(Action act) const
{
  for (auto &it : _actionKeys)
    {
      if (it.first == act)
	return it.second[0];
    }
  return sf::Keyboard::Unknown;
}

const std::string	&Controls::getCodeFromKey(ctrl::key k) const
{
  for (auto &it : _keycode)
    {
      if (it.second == k)
	return it.first;
    }
  throw (Exception("Key not bound"));
}

ctrl::key	Controls::getLastKey(Action act) const
{
  auto		it = _actionKeys.find(act);
  ctrl::key	last = sf::Keyboard::Unknown;

  if (it == _actionKeys.end())
    return sf::Keyboard::Unknown;
  for (auto ait = it->second.begin(); ait != it->second.end(); ++ait)
    {
      if (*ait == sf::Keyboard::Unknown)
	return last;
      last = *ait;
    }
  return last;
}

void	Controls::bindActionOnKey(ctrl::key k, Action act)
{
  auto	it = _actionKeys.find(act);

  if (it == _actionKeys.end())
    {
      _actionKeys.insert(std::pair<Action, std::array<ctrl::key, 5>> (act,
	{sf::Keyboard::Unknown, sf::Keyboard::Unknown, sf::Keyboard::Unknown,
	    sf::Keyboard::Unknown, sf::Keyboard::Unknown}));
      _actionKeys[act].front() = k;
      return ;
    }

  std::array<ctrl::key, 5>	&keys = it->second;
  for (auto &keyboardKey : keys)
    if (keyboardKey == sf::Keyboard::Unknown)
      {
	keyboardKey = k;
	return ;
      }
  for (unsigned int i = 0; i < keys.size() - 1; ++i)
    keys[i] = keys[i + 1];
  keys.back() = k;
}

void		Controls::pressKey(ctrl::key k)
{
  Action	act;

  _keyState[k] = true;
  act = getActionFromKey(k);
  if (static_cast<int>(act) < 0 || static_cast<unsigned int>(act) >= _actions.size())
    return ;

  t_action &action = _actions[static_cast<unsigned int>(act)];
  if (action.type == actionType::Toggle)
    action.state = !action.state;
  else
    action.state = true;
}

void		Controls::releaseKey(ctrl::key k)
{
  Action	act;

  _keyState[k] = false;
  act = getActionFromKey(k);
  if (static_cast<int>(act) < 0 || static_cast<unsigned int>(act) >= _actions.size())
    return ;

  t_action &action = _actions[static_cast<unsigned int>(act)];
  if (action.type != actionType::Toggle)
    action.state = false;
}
