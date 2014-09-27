#include <algorithm>
#include "Controls.hpp"
#include "Exception.hpp"

Controls::Controls()
{
  using namespace sf;

  _keycode["A"] = t_entry(Keyboard::A);
  _keycode["B"] = t_entry(Keyboard::B);
  _keycode["C"] = t_entry(Keyboard::C);
  _keycode["D"] = t_entry(Keyboard::D);
  _keycode["E"] = t_entry(Keyboard::E);
  _keycode["F"] = t_entry(Keyboard::F);
  _keycode["G"] = t_entry(Keyboard::G);
  _keycode["H"] = t_entry(Keyboard::H);
  _keycode["I"] = t_entry(Keyboard::I);
  _keycode["J"] = t_entry(Keyboard::J);
  _keycode["K"] = t_entry(Keyboard::K);
  _keycode["L"] = t_entry(Keyboard::L);
  _keycode["M"] = t_entry(Keyboard::M);
  _keycode["N"] = t_entry(Keyboard::N);
  _keycode["O"] = t_entry(Keyboard::O);
  _keycode["P"] = t_entry(Keyboard::P);
  _keycode["Q"] = t_entry(Keyboard::Q);
  _keycode["R"] = t_entry(Keyboard::R);
  _keycode["S"] = t_entry(Keyboard::S);
  _keycode["T"] = t_entry(Keyboard::T);
  _keycode["U"] = t_entry(Keyboard::U);
  _keycode["V"] = t_entry(Keyboard::V);
  _keycode["W"] = t_entry(Keyboard::W);
  _keycode["X"] = t_entry(Keyboard::X);
  _keycode["Y"] = t_entry(Keyboard::Y);
  _keycode["Z"] = t_entry(Keyboard::Z);
  _keycode["NUM0"] = t_entry(Keyboard::Num0);
  _keycode["NUM1"] = t_entry(Keyboard::Num1);
  _keycode["NUM2"] = t_entry(Keyboard::Num2);
  _keycode["NUM3"] = t_entry(Keyboard::Num3);
  _keycode["NUM4"] = t_entry(Keyboard::Num4);
  _keycode["NUM5"] = t_entry(Keyboard::Num5);
  _keycode["NUM6"] = t_entry(Keyboard::Num6);
  _keycode["NUM7"] = t_entry(Keyboard::Num7);
  _keycode["NUM8"] = t_entry(Keyboard::Num8);
  _keycode["NUM9"] = t_entry(Keyboard::Num9);
  _keycode["ESCAPE"] = t_entry(Keyboard::Escape);
  _keycode["L_CTRL"] = t_entry(Keyboard::LControl);
  _keycode["L_SHIFT"] = t_entry(Keyboard::LShift);
  _keycode["L_ALT"] = t_entry(Keyboard::LAlt);
  _keycode["L_SYSTEM"] = t_entry(Keyboard::LSystem);
  _keycode["R_CTRL"] = t_entry(Keyboard::RControl);
  _keycode["R_SHIFT"] = t_entry(Keyboard::RShift);
  _keycode["R_ALT"] = t_entry(Keyboard::RAlt);
  _keycode["R_SYSTEM"] = t_entry(Keyboard::RSystem);
  _keycode["MENU"] = t_entry(Keyboard::Menu);
  _keycode["LBRACKET"] = t_entry(Keyboard::LBracket);
  _keycode["RBRACKET"] = t_entry(Keyboard::RBracket);
  _keycode["SEMICOLON"] = t_entry(Keyboard::SemiColon);
  _keycode["COMMA"] = t_entry(Keyboard::Comma);
  _keycode["PERIOD"] = t_entry(Keyboard::Period);
  _keycode["SLASH"] = t_entry(Keyboard::Slash);
  _keycode["BACKSLASH"] = t_entry(Keyboard::BackSlash);
  _keycode["Tilde"] = t_entry(Keyboard::Tilde);
  _keycode["EQUAL"] = t_entry(Keyboard::Equal);
  _keycode["DASH"] = t_entry(Keyboard::Dash);
  _keycode["SPACE"] = t_entry(Keyboard::Space);
  _keycode["ENTER"] = t_entry(Keyboard::Return);
  _keycode["BACKSPACE"] = t_entry(Keyboard::BackSpace);
  _keycode["TAB"] = t_entry(Keyboard::Tab);
  _keycode["PGUP"] = t_entry(Keyboard::PageUp);
  _keycode["PGDN"] = t_entry(Keyboard::PageDown);
  _keycode["HOME"] = t_entry(Keyboard::Home);
  _keycode["INS"] = t_entry(Keyboard::Insert);
  _keycode["DEL"] = t_entry(Keyboard::Delete);
  _keycode["PLUS"] = t_entry(Keyboard::Add);
  _keycode["MINUS"] = t_entry(Keyboard::Subtract);
  _keycode["MULT"] = t_entry(Keyboard::Multiply);
  _keycode["DIV"] = t_entry(Keyboard::Divide);
  _keycode["LEFTARROW"] = t_entry(Keyboard::Left);
  _keycode["RIGHTARROW"] = t_entry(Keyboard::Right);
  _keycode["UPARROW"] = t_entry(Keyboard::Up);
  _keycode["DOWNARROW"] = t_entry(Keyboard::Down);
  _keycode["KP_0"] = t_entry(Keyboard::Numpad0);
  _keycode["KP_1"] = t_entry(Keyboard::Numpad1);
  _keycode["KP_2"] = t_entry(Keyboard::Numpad2);
  _keycode["KP_3"] = t_entry(Keyboard::Numpad3);
  _keycode["KP_4"] = t_entry(Keyboard::Numpad4);
  _keycode["KP_5"] = t_entry(Keyboard::Numpad5);
  _keycode["KP_6"] = t_entry(Keyboard::Numpad6);
  _keycode["KP_7"] = t_entry(Keyboard::Numpad7);
  _keycode["KP_8"] = t_entry(Keyboard::Numpad8);
  _keycode["KP_9"] = t_entry(Keyboard::Numpad9);
  _keycode["F1"] = t_entry(Keyboard::F1);
  _keycode["F2"] = t_entry(Keyboard::F2);
  _keycode["F3"] = t_entry(Keyboard::F3);
  _keycode["F4"] = t_entry(Keyboard::F4);
  _keycode["F5"] = t_entry(Keyboard::F5);
  _keycode["F6"] = t_entry(Keyboard::F6);
  _keycode["F7"] = t_entry(Keyboard::F7);
  _keycode["F8"] = t_entry(Keyboard::F8);
  _keycode["F9"] = t_entry(Keyboard::F9);
  _keycode["F10"] = t_entry(Keyboard::F10);
  _keycode["F11"] = t_entry(Keyboard::F11);
  _keycode["F12"] = t_entry(Keyboard::F12);
  _keycode["F13"] = t_entry(Keyboard::F13);
  _keycode["F14"] = t_entry(Keyboard::F14);
  _keycode["F15"] = t_entry(Keyboard::F15);
  _keycode["PAUSE"] = t_entry(Keyboard::Pause);

  _keycode["MOUSEL"] = t_entry(Mouse::Left, ctrl::type::Mouse);
  _keycode["MOUSER"] = t_entry(Mouse::Right, ctrl::type::Mouse);
  _keycode["MOUSEM"] = t_entry(Mouse::Middle, ctrl::type::Mouse);
  _keycode["MOUSE4"] = t_entry(Mouse::XButton1, ctrl::type::Mouse);
  _keycode["MOUSE5"] = t_entry(Mouse::XButton2, ctrl::type::Mouse);
  _keycode["MWHEELUP"] = t_entry(1, ctrl::type::MouseWheel);
  _keycode["MWHEELDOWN"] = t_entry(-1, ctrl::type::MouseWheel);

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

bool	Controls::isPressed(const t_entry &entry)
{
  return _keyState[static_cast<int>(entry.type)][entry.key];
}

Action	Controls::getActionFromKey(const t_entry &entry) const
{
  if (entry == ctrl::state::Unset)
    return Action::Unknown;
  for (auto &it : _actionKeys)
    {
      for (auto &elem : it.second)
	if (elem == entry)
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

t_entry	Controls::getKeyFromCode(const std::string &code) const
{
  auto	it = _keycode.find(code);

  return ((it == _keycode.end()) ? t_entry(sf::Keyboard::Unknown): it->second);
}

t_entry	Controls::getKeyFromAction(Action act) const
{
  for (auto &it : _actionKeys)
    {
      if (it.first == act)
	return it.second[0];
    }
  return t_entry(sf::Keyboard::Unknown);
}

t_entry	Controls::getLastKey(Action act) const
{
  auto		it = _actionKeys.find(act);
  t_entry	last = t_entry(sf::Keyboard::Unknown);

  if (it == _actionKeys.end())
    return last;
  for (auto ait = it->second.begin(); ait != it->second.end(); ++ait)
    {
      if (*ait == ctrl::state::Unset)
	return last;
      last = *ait;
    }
  return last;
}

const std::string	&Controls::getCodeFromKey(const t_entry &entry) const
{
  for (auto &it : _keycode)
    {
      if (it.second == entry)
	return it.first;
    }
  throw (Exception("Key not bound"));
}

void	Controls::bindActionOnKey(const t_entry &entry, Action act)
{
  auto	it = _actionKeys.find(act);

  if (it == _actionKeys.end())
    {
      _actionKeys.insert(std::pair<Action, std::array<t_entry, 5>> (act,
	{t_entry(sf::Keyboard::Unknown),
	    t_entry(sf::Keyboard::Unknown),
	    t_entry(sf::Keyboard::Unknown),
	    t_entry(sf::Keyboard::Unknown),
	    t_entry(sf::Keyboard::Unknown)}));
      _actionKeys[act].front() = entry;
      return ;
    }

  std::array<t_entry, 5>	&keys = it->second;
  for (auto &entryKey : keys)
    if (entryKey == ctrl::state::Unset)
      {
	entryKey = entry;
	return ;
      }
  for (unsigned int i = 0; i < keys.size() - 1; ++i)
    keys[i] = keys[i + 1];
  keys.back() = entry;
}

void		Controls::pressKey(const t_entry &entry)
{
  Action	act;

  _keyState[static_cast<int>(entry.type)][entry.key] = true;
  act = getActionFromKey(entry);
  if (static_cast<int>(act) < 0 || static_cast<unsigned int>(act) >= _actions.size())
    return ;
  t_action &action = _actions[static_cast<unsigned int>(act)];
  if (action.type == actionType::Toggle)
    action.state = !action.state;
  else
    action.state = true;
}

void		Controls::releaseKey(const t_entry &entry)
{
  Action	act;

  _keyState[static_cast<int>(entry.type)][entry.key] = false;
  act = getActionFromKey(entry);
  if (static_cast<int>(act) < 0 || static_cast<unsigned int>(act) >= _actions.size())
    return ;

  t_action &action = _actions[static_cast<unsigned int>(act)];
  if (action.type != actionType::Toggle)
    action.state = false;
}
