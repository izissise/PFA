#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <set>
#include <string>

typedef struct s_event t_event;

class IObserver
{
public:
  virtual void	trigger(const t_event &event) = 0;
};

class Observable
{
public:
  Observable();
  ~Observable();

  void	notify(const t_event &event) const;
  void	addObserver(IObserver * const observer);
  void	removeObserver(IObserver * const observer);

private:
  std::set<IObserver *>		_observers;
};

#endif /* _OBSERVER_H_ */
