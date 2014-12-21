#ifndef _BLINKTEXT_H_
#define _BLINKTEXT_H_

#include <SFML/System/Clock.hpp>

class BlinkText
{
public:
  BlinkText(unsigned int blinkTime = 700);
  ~BlinkText();

  void	update();
  bool	getDisplayState() const;

private:
  sf::Clock	_time;
  unsigned int	_blinkTime;
  bool		_display;
};

#endif /* _BLINKTEXT_H_ */
