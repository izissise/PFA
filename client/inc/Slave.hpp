#ifndef _SLAVE_H_
#define _SLAVE_H_

#include <vector>

typedef struct s_event	t_event;
enum class wEvent;

class ISlave
{
public:
  virtual void	getNotification(const t_event &event) = 0;
};


typedef struct	s_slave
{
  ISlave	*slave;
  wEvent	flag;
  s_slave(ISlave * const s, wEvent f) :
    slave(s), flag(f)
  {
  }
}		t_slave;


class Master
{
public:
  Master();
  virtual ~Master();

  void	informSlaves(const t_event &event) const;
  void	addSlave(const t_slave &slave);
  void	removeSlave(ISlave * const slave);

private:
  std::vector<t_slave>		_slaves;
};

#endif /* _SLAVE_H_ */
