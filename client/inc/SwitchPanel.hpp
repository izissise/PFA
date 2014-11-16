#ifndef _SWITCHPANEL_H_
#define _SWITCHPANEL_H_

#include "Panel.hpp"

class SwitchPanel : public APanelScreen
{
public:
  enum Zone
    {
      Left,
      Middle,
      Right
    };

public:
  SwitchPanel(const sf::FloatRect &panelZone, const sf::FloatRect &contentZone);

  void	construct(const sf::Texture &texture, Settings &set,
		  const std::vector<APanelScreen *> &panels);
  void	draw(sf::RenderWindow &window, bool toWin);
  int	update(const sf::Event &event, sf::RenderWindow &ref, Settings &set);

  void	trigger(const t_event &event);
  void	addContent(AWidget * const widget);
  void	addContent(const std::initializer_list<AWidget * const> &widgets);
  void	addSides(const std::array<AWidget * const, 2> &sides);
  const sf::FloatRect	&getLeftZone() const;
  const sf::FloatRect	&getRightZone() const;
  const sf::FloatRect	&getContentZone() const;

private:
  void		switchContent(int dir);
  unsigned int	getContentSize() const;
  bool		isOver(const sf::RenderWindow &ref) const;
  int		isOverContent(const sf::RenderWindow &ref) const;
  bool		isClicked(const sf::Event &event, sf::Mouse::Button button) const;

private:
  unsigned int		_idx;
  Panel			*_content;
  Panel			*_leftSide;
  Panel			*_rightSide;
};

#endif /* _SWITCHPANEL_H_ */
