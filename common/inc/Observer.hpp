#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <set>
#include <string>

class IObserver
{
public:
  virtual void	trigger(const std::string &event) = 0;
};

class Observable
{
public:
  Observable();
  ~Observable();

  void	notify(const std::string &data) const;
  void	addObserver(IObserver * const observer);
  void	removeObserver(IObserver * const observer);

private:
  std::set<IObserver *>		_observers;
};

#endif /* _OBSERVER_H_ */
