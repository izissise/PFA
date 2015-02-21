#ifndef _CLIENTENTITY_H_
#define _CLIENTENTITY_H_

#include "Unused.hpp"

#include "AMovable.hpp"

class ClientEntity : public AMovable
{
public:
  ClientEntity() = default;
  virtual ~ClientEntity() {};

  void update(std::chrono::milliseconds const & timeStep) override;

};

#endif /* _CLIENTENTITY_H_ */
