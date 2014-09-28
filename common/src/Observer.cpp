#include <algorithm>
#include "Observer.hpp"

Observable::Observable()
{
}

Observable::~Observable()
{
}

void	Observable::addObserver(IObserver * const observer)
{
  _observers.push_back(observer);
}

void	Observable::addObserver(const std::initializer_list<IObserver * const> &observers)
{
  for (auto &observer : observers)
    _observers.push_back(observer);
}

void	Observable::removeObserver(IObserver * const observer)
{
  auto	it = std::find(_observers.begin(), _observers.end(), observer);

  if (it != _observers.end())
    _observers.erase(it);
}

void	Observable::notify(const t_event &message) const
{
  for (auto &observer : _observers)
    observer->trigger(message);
}

IObserver	*Observable::getObserver(unsigned int idx)
{
  return _observers[idx];
}
