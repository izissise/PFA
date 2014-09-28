#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <vector>

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
  void	addObserver(const std::initializer_list<IObserver * const> observers);
  void	removeObserver(IObserver * const observer);
  IObserver	*getObserver(unsigned int idx);

private:
  std::vector<IObserver *>		_observers;
};

#endif /* _OBSERVER_H_ */
