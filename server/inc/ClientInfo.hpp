#ifndef _CLIENTINFO_H_
#define _CLIENTINFO_H_

#include <string>
#include "Vector2.hpp"

class ClientInfo
{
public:
  ClientInfo();
  virtual ~ClientInfo();

  void		setId(const std::string &id);

  const std::string	&getId() const;

private:
  std::string	_id;
};

#endif /* _CLIENTINFO_H_ */
