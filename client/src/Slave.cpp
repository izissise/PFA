#include <algorithm>
#include "AWidget.hpp"
#include "Slave.hpp"

Master::Master()
{
}

Master::~Master()
{
}

void	Master::addSlave(const t_slave &slave)
{
  _slaves.push_back(slave);
}

void	Master::removeSlave(ISlave * const slave)
{
  auto it = std::find_if(_slaves.begin(), _slaves.end(),
			 [slave](const t_slave &elem) { return elem.slave == slave; });
  if (it == _slaves.end())
    return ;
  _slaves.erase(it);
}

void	Master::informSlaves(const t_event &event) const
{
  for (auto &elem : _slaves)
    {
      if (elem.flag & event.e)
	elem.slave->getNotification(event);
    }
}
