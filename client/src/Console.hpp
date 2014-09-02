#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <SFML/Graphics.hpp>
#include "Settings.hpp"

class Console
{
public:
  Console(Settings * const set);
  virtual ~Console();

  void		run(sf::RenderWindow &window);
private:
  Settings		*_set;
};

#endif /* _CONSOLE_H_ */
