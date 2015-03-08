#ifndef _LINES_H_
#define _LINES_H_

#include <vector>
#include "Vector2.hpp"

class Lines
{
public:
  Lines();
  virtual ~Lines();

  void			clear();
  const	Vector2f	&getPoint(unsigned int idx);
  unsigned int		size() const;

  std::vector<Vector2f>	points;
};

#endif /* _LINES_H_ */
