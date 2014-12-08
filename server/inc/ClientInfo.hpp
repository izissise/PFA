#ifndef _CLIENTINFO_H_
#define _CLIENTINFO_H_

#include "Vector2.hpp"

class ClientInfo
{
public:
  ClientInfo();
  virtual ~ClientInfo();

  void		setResolution(const Vector2u &screenRes);

  const Vector2u	&getResolution() const;

private:
  Vector2u	_screenRes;
};

#endif /* _CLIENTINFO_H_ */
