#include "Observer.hpp"

Observable::Observable()
{
}

Observable::~Observable()
{
}

void	Observable::addObserver(IObserver * const observer)
{
  _observers.insert(observer);
}

void	Observable::removeObserver(IObserver * const observer)
{
  _observers.erase(observer);
}

void	Observable::notify(const std::string &message) const
{
  for (auto &observer : _observers)
    observer->update(message);
}
