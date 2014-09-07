#ifndef _AWIDGET_H_
#define _AWIDGET_H_

#include <SFML/Graphics.hpp>
#include <functional>
#include "Settings.hpp"

class AWidget
{
public:
  AWidget(const std::string &id, const sf::FloatRect &zone, sf::Text *text);
  virtual ~AWidget();

  virtual int	update(const sf::Event &event, sf::RenderWindow &ref, Settings &set) = 0;
  virtual void	draw(sf::RenderWindow &window) const;

  void		addSprite(sf::Sprite &sprite);
  void		addSprite(sf::Texture * const texture, const sf::IntRect &rect);
  void		alignText(const sf::Vector2f &pos, const sf::Vector2f &size,
			  float xPercent, float yPercent);
  void		setTextPosition(int x, int y);
  void		setTextAttr(unsigned int style);
  void		setFunction(const std::function<int (AWidget &widget, const sf::Event &event,
						     sf::RenderWindow &ref)> &func);

public:		// public so the lambda can call it
  bool		isOver(const sf::RenderWindow &ref) const;
  bool		isClicked(const sf::Event &event, sf::Mouse::Button button) const;

protected:
  const std::string		_id;
  sf::FloatRect			_zone;
  std::vector<sf::Sprite>	_sprites;
  sf::Text			*_text;
  std::function<int (AWidget &widget, const sf::Event &event, sf::RenderWindow &ref)>	_update;
};

#endif /* _AWIDGET_H_ */
