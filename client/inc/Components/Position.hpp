#ifndef	POSITION_H
# define POSITION_H

# include <entityx/entityx.h>

struct Position : entityx::Component<Position>
{
  Position(float p_x, float p_y) : x(p_x), y(p_y) {}

  float x;
  float y;
};

#endif /* POSITION_H */
