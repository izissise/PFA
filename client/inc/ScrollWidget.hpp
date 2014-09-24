#ifndef _SCROLLWIDGET_H_
#define _SCROLLWIDGET_H_

#include "AWidget.hpp"
#include "APanelScreen.hpp"

#define PADDINGBOTTOM 50

enum class Scroll
{
  Vertical = 0,
    Horizontal = 1
    };

class ScrollWidget : public AWidget
{
public:
  ScrollWidget(const std::string &id, const sf::FloatRect &zone,
	       Scroll dir, APanelScreen *panel,
	       const sf::Text &text = sf::Text(),
	       wFlag flg = wFlag::Movable | wFlag::Resizable);

  int   update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);
  bool	isOver(const sf::RenderWindow &ref) const;
  void		addSprite(t_sprite &elem);
  void		addSprite(const sf::Texture &texture, const sf::IntRect &rect,
			  bool display = true);
  void		toSize(unsigned int spritePos, float pX, float pY);
  void		updateScrollSize();

private:
  void	movePicker(sf::Sprite &sprite, float x, float y);
  void	moveWidgets(sf::Vector2f moveSize);
  int	handleMouse(float pX, float pY);

private:
  bool		_active;
  Scroll	_dir;
  APanelScreen	*_panel;
  float		_ratio;
};

#endif /* _SCROLLWIDGET_H_ */
