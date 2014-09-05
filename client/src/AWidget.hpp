#ifndef _AWIDGET_H_
#define _AWIDGET_H_

#include <SFML/Graphics.hpp>

class AWidget
{
public:
  AWidget(const std::string &id, const sf::Vector2u &pos, sf::Text *text);
  virtual ~AWidget();

  virtual int	update(sf::Event event) = 0;
  virtual void	draw(sf::RenderWindow &window) const;

  void		addSprite(sf::Sprite &sprite);

protected:
  const std::string		_id;
  sf::Vector2u			_pos;
  std::vector<sf::Sprite>	_sprites;
  sf::Text			*_text;
};

#endif /* _AWIDGET_H_ */
