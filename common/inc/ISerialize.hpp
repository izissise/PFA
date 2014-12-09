#ifndef _ISERIALIZE_HPP_
# define _ISERIALIZE_HPP_

# include "ProtocolMessage.pb.h"
# include "ClientMessage.pb.h"

class ISerialize
{
public:
  virtual ~ISerialize() { };
  virtual std::string serialize() = 0;
};

#endif
