#include "ClientInfo.hpp"

ClientInfo::ClientInfo()
{
}

ClientInfo::~ClientInfo()
{
}

void	ClientInfo::setId(const std::string &id)
{
  _id = id;
}

const std::string	&ClientInfo::getId() const
{
  return _id;
}
