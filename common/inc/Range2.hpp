#ifndef	RANGE2_H
# define RANGE2_H

# include <iterator>

# include "Vector2.hpp"

template<typename T>
class Range2
{
public:
  Range2(void) {}
  Range2(const Vector2<T>& p_origin, const Vector2<T>& p_dest) : _origin(p_origin), _dest(p_dest) {
    if (not (_origin < _dest) and _dest != _origin) {
      throw std::invalid_argument("origin is greater than destination");
    }
  }
  virtual ~Range2(void) = default;
  Range2(const Range2& rhs) { *this = rhs; };
  Range2&	operator=(const Range2& rhs) {
    _origin = rhs._origin;
    _dest = rhs._dest;
    if (not (_origin < _dest) and _dest != _origin) {
      throw std::invalid_argument("origin is greater than destination");
    }
    return *this;
  }

  const Vector2<T>&	origin(void) const { return _origin; }
  const Vector2<T>&	dest(void) const { return _dest; }

  T	left(void) const	{ return _origin.x; }
  T	right(void) const	{ return _dest.x; }
  T	top(void) const		{ return _origin.y; }
  T	bottom(void) const	{ return _dest.y; }
  T	width(void) const	{ return _dest.x - _origin.x + 1; }
  T	height(void) const	{ return _dest.y - _origin.y + 1; }
  unsigned size(void) const	{ return this->width() * this->height(); }

  bool			operator==(const Range2& rhs) const {
    return (_origin == rhs._origin and _dest == rhs._dest);
  }
  bool			operator!=(const Range2& rhs) const {
    return (_origin != rhs._origin or _dest != rhs._dest);
  }

  class Iterator : public std::iterator<std::forward_iterator_tag, Vector2<T>>
  {
  public:
    Iterator(const Iterator& rhs) : _container(rhs._container), _cursor(rhs._cursor) {}
    ~Iterator(void) = default;

    Iterator&	operator++() { _next(); return *this; }
    Iterator	operator++(int) { Iterator tmp(*this); _next(); return tmp; }

    bool	operator==(const Iterator& rhs) const { return _cursor == rhs._cursor; }
    bool	operator!=(const Iterator& rhs) const { return _cursor != rhs._cursor; }
    Vector2<T>&	operator*(void) { return _cursor; }

  private:
    friend class Range2;

    Iterator(Range2& container, const Vector2<T>& cursor) : _container(container), _cursor(cursor) {}

    void _next(void) {
      ++_cursor.x;
      if (_cursor.x > _container.dest().x) {
	_cursor.x = _container.origin().x;
	++_cursor.y;
      }
    }

    Range2&	_container;
    Vector2<T>	_cursor;
  };

  class ConstIterator : public std::iterator<std::forward_iterator_tag, Vector2<T>>
  {
  public:
    ConstIterator(const ConstIterator& rhs) : _container(rhs._container), _cursor(rhs._cursor) {}
    ~ConstIterator(void) = default;

    ConstIterator&	operator++() { _next(); return *this; }
    ConstIterator	operator++(int) { ConstIterator tmp(*this); _next(); return tmp; }

    bool		operator==(const ConstIterator& rhs) const { return _cursor == rhs._cursor; }
    bool		operator!=(const ConstIterator& rhs) const { return _cursor != rhs._cursor; }
    const Vector2<T>&	operator*(void) { return _cursor; }

  private:
    friend class Range2;

    ConstIterator(const Range2& container, const Vector2<T>& cursor) : _container(container), _cursor(cursor) {}

    void _next(void) {
      ++_cursor.x;
      if (_cursor.x > _container.dest().x) {
	_cursor.x = _container.origin().x;
	++_cursor.y;
      }
    }

    const Range2&	_container;
    Vector2<T>		_cursor;
  };

  typedef Iterator	iterator;
  typedef ConstIterator	const_iterator;

  iterator		begin(void) { return {*this, _origin}; }
  iterator		end(void) { return {*this, {_origin.x, _dest.y + 1}}; }
  const_iterator	begin(void) const { return const_iterator(*this, _origin); }
  const_iterator	end(void) const { return const_iterator(*this, {_origin.x, _dest.y + 1}); }

private:
  Vector2<T>	_origin;
  Vector2<T>	_dest;
};

typedef Range2<float>		Range2f;
typedef Range2<double>		Range2d;
typedef Range2<int>		Range2i;
typedef Range2<unsigned>	Range2u;
typedef Range2<long>		Range2l;
typedef Range2<char>		Range2c;

#endif /* RANGE2_H */
