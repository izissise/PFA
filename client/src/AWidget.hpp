#ifndef _AWIDGET_H_
#define _AWIDGET_H_

#include <SFML/Graphics.hpp>

class AWidget
{
public:
  AWidget(const std::string &id, const sf::Vector2f &pos, sf::Text *text);
  virtual ~AWidget();

  virtual int	update(const sf::Event &event) = 0;
  virtual void	draw(sf::RenderWindow &window) const;

  void		addSprite(sf::Sprite &sprite);
  void		addSprite(sf::Texture * const texture, const sf::IntRect &rect);
  void		alignText(const sf::Vector2f &pos, const sf::Vector2f &size,
			  float xPercent, float yPercent);
  void		setTextPosition(int x, int y);
  void		setTextAttr(unsigned int style);

protected:
  bool		isOver() const;
  bool		isClicked(const sf::Event &event) const;

protected:
  const std::string		_id;
  sf::Vector2f			_pos;
  std::vector<sf::Sprite>	_sprites;
  sf::Text			*_text;
};

#endif /* _AWIDGET_H_ */
