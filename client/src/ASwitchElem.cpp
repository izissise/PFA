#include "ASwitchElem.hpp"

ASwitchElem::ASwitchElem(const sf::FloatRect &panelZone,
			 const sf::FloatRect &contentZone) :
  APanelScreen(panelZone), _idx(0)
{
  _content = new Panel(contentZone);
  _leftSide = new Panel(sf::FloatRect{_zone.left, _zone.top,
	(_zone.width - contentZone.width) / 2.f, _zone.height});
  _rightSide = new Panel(sf::FloatRect{
      (_zone.left + _zone.width - (_zone.width - contentZone.width) / 2.f), _zone.top,
	(_zone.width - contentZone.width) / 2.f, _zone.height});
}

void	ASwitchElem::construct(const sf::Texture &texture, Settings &set,
			       const std::vector<APanelScreen *> &panels)
{
  _content->construct(texture, set, panels);
  _leftSide->construct(texture, set, panels);
  _rightSide->construct(texture, set, panels);
  addPanel({_leftSide, _rightSide});
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void	ASwitchElem::addSides(const std::array<AWidget * const, 2> &sides)
{
  _leftSide->addWidget(sides[0]);
  _rightSide->addWidget(sides[1]);
}

void		ASwitchElem::switchContent(int dir)
{
  unsigned int	contentSize;

  contentSize = getContentSize();
  if (contentSize == 0)
    return ;
  _idx = ((dir == 1) ? ((_idx + 1) % contentSize) : ((contentSize + _idx - 1) % contentSize));
}

void	ASwitchElem::trigger(const t_event &event)
{
  if (event.e & wEvent::Update)
    switchContent(event.idx);
  else
    APanelScreen::trigger(event);
}

const sf::FloatRect	&ASwitchElem::getLeftZone() const
{
  return _leftSide->getZone();
}

const sf::FloatRect	&ASwitchElem::getRightZone() const
{
  return _rightSide->getZone();
}

const sf::FloatRect	&ASwitchElem::getContentZone() const
{
  return _content->getZone();
}

bool	ASwitchElem::isOver(const sf::RenderWindow &ref) const
{
  return _zone.contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(ref)));
}

int	ASwitchElem::isOverContent(const sf::RenderWindow &ref) const
{
  const std::vector<APanelScreen *>	&vec = getSubPanels();

  if (vec[0]->getZone().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(ref))))
    return -1;
  else if (vec[1]->getZone().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(ref))))
    return 1;
  return 0;
}

bool	ASwitchElem::isClicked(const sf::Event &event, sf::Mouse::Button button) const
{
  if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == button)
	return true;
    }
  return false;
}
