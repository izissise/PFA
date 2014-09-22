#ifndef _SCROLLWIDGET_H_
#define _SCROLLWIDGET_H_

#include "AWidget.hpp"

enum class Scroll
{
  Vertical = 0,
    Horizontal = 1
    };

class ScrollWidget : public AWidget
{
public:
  ScrollWidget(const std::string &id, const sf::FloatRect &zone,
	       Scroll dir, const sf::Text &text = sf::Text());

  int   update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
  bool	isOver(const sf::RenderWindow &ref) const;
  void		addSprite(t_sprite &elem);
  void		addSprite(const sf::Texture &texture, const sf::IntRect &rect,
			  bool display = true);

private:
  void	movePicker(sf::Sprite &sprite, float x, float y);

private:
  bool		_active;
  Scroll	_dir;
};

#endif /* _SCROLLWIDGET_H_ */
