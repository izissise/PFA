#ifndef _SWITCHELEM_H_
#define _SWITCHELEM_H_

#include "Panel.hpp"

class ASwitchElem : public APanelScreen
{
public:
  ASwitchElem(const sf::FloatRect &panelZone, const sf::FloatRect &contentZone);

  virtual int	update(const sf::Event &event,
		       sf::RenderWindow &ref,
		       Settings &set) = 0;

  void	construct(const sf::Texture &texture, Settings &set,
		  const std::vector<APanelScreen *> &panels);
  void	trigger(const t_event &event);
  void	addSides(const std::array<AWidget * const, 2> &sides);
  const sf::FloatRect	&getLeftZone() const;
  const sf::FloatRect	&getRightZone() const;
  const sf::FloatRect	&getContentZone() const;

protected:
  virtual unsigned int	getContentSize() const = 0;
  void		switchContent(int dir);
  bool		isOver(const sf::RenderWindow &ref) const;
  int		isOverContent(const sf::RenderWindow &ref) const;
  bool		isClicked(const sf::Event &event, sf::Mouse::Button button) const;

protected:
  unsigned int		_idx;
  Panel			*_content;

private:
  Panel			*_leftSide;
  Panel			*_rightSide;
};

#endif /* _SWITCHELEM_H_ */
