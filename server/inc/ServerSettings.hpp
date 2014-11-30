#ifndef _SERVERSETTINGS_HPP_
# define _SERVERSETTINGS_HPP_

# include "Cvar.hpp"
# include "Exception.hpp"
# include "Observer.hpp"

typedef struct  s_event
{
  std::string   change;
}               t_event;

class ServerSettings : public CvarList, public Observable
{
public:
  ServerSettings();
  virtual ~ServerSettings();

  void	setCvar(const std::string &name, const std::string &value);
};

#endif
