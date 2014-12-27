#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include <SFML/System.hpp>

class Countdown
{
public:
  Countdown();
  ~Countdown() = default;

  void	start(const sf::Time &time);
  bool	update() const;
private:
  sf::Clock	_clock;
  sf::Time	_time;
};

#endif /* _COUNTDOWN_H_ */
