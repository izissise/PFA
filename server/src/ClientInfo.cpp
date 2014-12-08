#include "ClientInfo.hpp"

ClientInfo::ClientInfo()
{
}

ClientInfo::~ClientInfo()
{
}

void	ClientInfo::setResolution(const Vector2u &screenRes)
{
  _screenRes = screenRes;
}

const Vector2u	&ClientInfo::getResolution() const
{
  return _screenRes;
}
