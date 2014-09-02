#include <cctype>
#include "UserInput.hpp"
#include "Exception.hpp"

UserInput::UserInput(int maxSize) : _maxSize(maxSize)
{
}

UserInput::~UserInput()
{
}

bool	UserInput::getInput(const sf::Event &event)
{
  key	k = 0;

  if (event.type == sf::Event::TextEntered && _input.getSize() < _maxSize)
    {
      if (isprint(event.text.unicode))
	_input.insert(_input.getSize(), event.text.unicode);
    }
  else if (event.type == sf::Event::KeyPressed)
    {
      k = event.key.code;
      if (event.key.code == sf::Keyboard::BackSpace && _input.getSize() > 0)
	_input.erase(_input.getSize() - 1);
    }
  return (k == sf::Keyboard::Return);
}

const sf::String	&UserInput::getString() const
{
  return _input;
}

void	UserInput::clear()
{
  _input.clear();
}
