#include "Countdown.hpp"

Countdown::Countdown()
{
}

void	Countdown::start(const sf::Time &time)
{
  _time = time;
  _clock.restart();
}

bool	Countdown::update() const
{
  return (_clock.getElapsedTime().asMicroseconds() >= _time.asMicroseconds());
}
