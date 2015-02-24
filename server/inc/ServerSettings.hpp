#ifndef _SERVERSETTINGS_HPP_
# define _SERVERSETTINGS_HPP_

# include "ACvar.hpp"
# include "Exception.hpp"
# include "Observer.hpp"
# include "ISerialize.hpp"

class ServerSettings : public ACvar, public Observable, public ISerialize
{
public:
  ServerSettings();
  virtual ~ServerSettings();

  void	setCvar(const std::string &name, const std::string &value);
  virtual std::string serialize();
};

#endif
