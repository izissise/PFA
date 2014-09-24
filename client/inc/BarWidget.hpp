#ifndef _BARWIDGET_H_
#define _BARWIDGET_H_

#include "AWidget.hpp"

class BarWidget : public AWidget
{
public:
  BarWidget(const std::string &id, const sf::FloatRect &zone,
	 const sf::Text &text = sf::Text());

  int   update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
  bool	isOver(const sf::RenderWindow &ref) const;
  void		addSprite(t_sprite &elem);
  void		addSprite(const sf::Texture &texture, const sf::IntRect &rect,
			  bool display = true);

private:
  void	movePicker(sf::Sprite &sprite, float x, float y);

private:
  bool	_active;
};

#endif /* _BARWIDGET_H_ */
