#include "Historical.hpp"

Historical::Historical(unsigned int s) :
  _maxSize(s), _pos(0)
{
}

Historical::~Historical()
{
}

void	Historical::addString(const sf::String &str)
{
  _content.push_front(str);
  if (_content.size() > _maxSize)
    _content.pop_back();
}

void	Historical::clear()
{
  _content.clear();
  _pos = 0;
}

const std::deque<sf::String>	&Historical::getContent() const
{
  return _content;
}

unsigned int	Historical::getMaxSize() const
{
  return _maxSize;
}

unsigned int	Historical::getPos() const
{
  return _pos;
}

unsigned int	Historical::getSize() const
{
  return _content.size();
}

void	Historical::setPos(unsigned int pos)
{
  _pos = pos;
}
