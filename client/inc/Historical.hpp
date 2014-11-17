#ifndef _HISTORICAL_H_
#define _HISTORICAL_H_

#include <SFML/Graphics.hpp>
#include <deque>

class Historical
{
public:
  Historical(unsigned int s = 100);
  virtual ~Historical();

  void			addString(const sf::String &str);
  void			clear();

  const std::deque<sf::String>	&getContent() const;
  unsigned int		getMaxSize() const;
  unsigned int		getPos() const;
  unsigned int		getSize() const;

  void			setPos(unsigned int pos);

private:
  std::deque<sf::String>	_content;
  unsigned int			_maxSize;
  unsigned int			_pos;
};

#endif /* _HISTORICAL_H_ */
