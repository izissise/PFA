#ifndef _SCROLLWIDGET_H_
#define _SCROLLWIDGET_H_

#include "AWidget.hpp"
#include "APanelScreen.hpp"

#define PADDINGBOTTOM 50

/*
** No unit for this, the highest the fastest
*/
#define SLIDESPEED 8
#define BIGGEST(x, y) ((x) > (y) ? (x) : (y))

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
  void	addSprite(t_sprite &elem);
  void	addSprite(const sf::Texture &texture, const sf::IntRect &rect,
		  bool display = true);
  void	toSize(unsigned int spritePos, float pX, float pY);

private:
  bool	isOver(const sf::RenderWindow &ref, unsigned int spritePos) const;
  void	movePicker(sf::Sprite &sprite, float x, float y);
  void	moveSubPanels(APanelScreen * const pan, const sf::Vector2f &moveSize);
  void	moveWidgets(APanelScreen * const pan, const sf::Vector2f &moveSize);
  int	handleMouse(int pX, int pY);
  void		updateScrollSize();
  void		updateButtonPos();
  unsigned int	calcPanelSize(APanelScreen * const pan, float diff,
			      const sf::FloatRect &barZone);
  unsigned int	getBiggest(const sf::FloatRect &barZone,
			   float diff,
			   const std::vector<AWidget *> &widgetList);

private:
  bool		_active;
  Scroll	_dir;
  APanelScreen	*_panel;
  float		_ratio;
  sf::Vector2i	_mousePos;
};

#endif /* _SCROLLWIDGET_H_ */
