#ifndef	VECTOR2_H
# define VECTOR2_H

template<typename T>
class Vector2
{
public:

  Vector2() : x(static_cast<T>(0)), y(static_cast<T>(0)) {}

  template <typename U>
  Vector2(const U& value) : x(static_cast<T>(value)), y(static_cast<T>(value)) {}

  template <typename U, typename V>
  Vector2(const U& p_x, const V& p_y) : x(static_cast<T>(p_x)), y(static_cast<T>(p_y)) {}

  template <typename U>
  Vector2(const Vector2<U>& vec) :
    x(static_cast<T>(vec.x)), y(static_cast<T>(vec.y))
  {}

  virtual ~Vector2() {};

  template <typename U>
  Vector2<T>&	operator=(const Vector2<U>& rhs) {
    x = static_cast<T>(rhs.x);
    y = static_cast<T>(rhs.y);
    return *this;
  }

  template <typename U>
  Vector2<T>	operator+(const U& rhs) const {
    Vector2<T> ret(*this);
    ret += rhs;
    return ret;
  }

  template <typename U>
  Vector2<T>	operator-(const U& rhs) const {
    Vector2<T> ret(*this);
    ret -= rhs;
    return ret;
  }

  template <typename U>
  Vector2<T>	operator*(const U& rhs) const {
    Vector2<T> ret(*this);
    ret *= rhs;
    return ret;
  }

  template <typename U>
  Vector2<T>	operator/(const U& rhs) const {
    Vector2<T> ret(*this);
    ret /= rhs;
    return ret;
  }

  template <typename U>
  Vector2<T>	operator%(const U& rhs) const {
    Vector2<T> ret(*this);
    ret %= rhs;
    return ret;
  }

  template <typename U>
  Vector2<T>&	operator+=(const Vector2<U>& rhs) { x += rhs.x; y += rhs.y; return *this; }

  template <typename U>
  Vector2<T>&	operator-=(const Vector2<U>& rhs) { x -= rhs.x; y -= rhs.y; return *this; }

  template <typename U>
  Vector2<T>&	operator*=(const Vector2<U>& rhs) { x *= rhs.x; y *= rhs.y; return *this; }

  template <typename U>
  Vector2<T>&	operator/=(const Vector2<U>& rhs) { x /= rhs.x; y /= rhs.y; return *this; }

  template <typename U>
  Vector2<T>&	operator%=(const Vector2<U>& rhs) { x %= rhs.x; y %= rhs.y; return *this; }

  template <typename U>
  Vector2<T>&	operator*=(const U& rhs) { x *= rhs; y *= rhs; return *this; }

  template <typename U>
  Vector2<T>&	operator/=(const U& rhs) { x /= rhs; y /= rhs; return *this; }

  template <typename U>
  Vector2<T>&	operator%=(const U& rhs) { x %= rhs; y %= rhs; return *this; }

  template <typename U>
  bool		operator==(const Vector2<U>& rhs) const { return (x == rhs.x && y == rhs.y); }

  template <typename U>
  bool		operator!=(const Vector2<U>& rhs) const { return !(*this == rhs); }

  template <typename U>
  bool		operator<(const Vector2<U>& rhs) const {
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
