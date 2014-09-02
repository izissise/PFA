#ifndef _USERINPUT_H_
#define _USERINPUT_H_

# include <SFML/Graphics.hpp>
# include <string>

typedef unsigned int	key;

class UserInput
{
public:
  UserInput(int maxSize = 2048);
  virtual ~UserInput();

  bool			getInput(const sf::Event &event);
  const sf::String	&getString() const;
  void			clear();

private:
  sf::String	_input;
  unsigned int	_maxSize;
};

#endif /* _USERINPUT_H_ */
