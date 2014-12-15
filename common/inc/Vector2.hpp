#ifndef	VECTOR2_H
# define VECTOR2_H

template<typename T>
class Vector2
{
public:
  Vector2(void) {}
  Vector2(const T& value) : x(value), y(value) {}
  Vector2(const T& p_x, const T& p_y) : x(p_x), y(p_y) {}
  Vector2(const Vector2& rhs) : x(rhs.x), y(rhs.y) {}
  template <typename U>
  explicit Vector2(const Vector2<U>& vec) :
    x(static_cast<T>(vec.x)),
    y(static_cast<T>(vec.y))
  {
  }
  Vector2&	operator=(const Vector2& rhs) {
    x = rhs.x;
    y = rhs.y;
    return *this;
  }

  Vector2	operator+(const Vector2& rhs) const { return {x + rhs.x, y + rhs.y}; }
  Vector2	operator-(const Vector2& rhs) const { return {x - rhs.x, y - rhs.y}; }
  Vector2	operator*(const Vector2& rhs) const { return {x * rhs.x, y * rhs.y}; }
  Vector2	operator/(const Vector2& rhs) const { return {x / rhs.x, y / rhs.y}; }
  Vector2	operator%(const Vector2& rhs) const { return {x % rhs.x, y % rhs.y}; }

  Vector2	operator*(const T& rhs) const { return {x * rhs, y * rhs}; }
  Vector2	operator/(const T& rhs) const { return {x / rhs, y / rhs}; }
  Vector2	operator%(const T& rhs) const { return {x % rhs, y % rhs}; }

  Vector2&	operator+=(const Vector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
  Vector2&	operator-=(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
  Vector2&	operator*=(const Vector2& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
  Vector2&	operator/=(const Vector2& rhs) { x /= rhs.x; y /= rhs.y; return *this; }
  Vector2&	operator%=(const Vector2& rhs) { x %= rhs.x; y %= rhs.y; return *this; }

  Vector2&	operator*=(const T& rhs) { x *= rhs; y *= rhs; return *this; }
  Vector2&	operator/=(const T& rhs) { x /= rhs; y /= rhs; return *this; }
  Vector2&	operator%=(const T& rhs) { x %= rhs; y %= rhs; return *this; }

  bool		operator==(const Vector2& rhs) const { return (x == rhs.x && y == rhs.y); }
  bool		operator!=(const Vector2& rhs) const { return (x != rhs.x || y != rhs.y); }

  bool		operator<(const Vector2& rhs) const {
    return (y < rhs.y || (y == rhs.y && x < rhs.x));
  }

  union {
    T	x;
    T	u;
    T	w;
    T	width;
  };
  union {
    T	y;
    T	v;
    T	h;
    T	height;
  };
};

typedef Vector2<float>		Vector2f;
typedef Vector2<double>		Vector2d;
typedef Vector2<int>		Vector2i;
typedef Vector2<unsigned>	Vector2u;
typedef Vector2<long>		Vector2l;
typedef Vector2<char>		Vector2c;

#endif /* VECTOR2_H */
