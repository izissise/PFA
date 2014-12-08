#ifndef	VELOCITY_H
# define VELOCITY_H

# include <entityx/Component.hpp>

struct Velocity : public entityx::Component<Velocity>
{
  Velocity(float p_x, float p_y) : x(p_x), y(p_y) {}

  float x;
  float y;
}

#endif /* VELOCITY_H */
