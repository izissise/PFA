#include "SwitchPanel.hpp"

SwitchPanel::SwitchPanel(const sf::FloatRect &panelZone,
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

void	SwitchPanel::construct(const sf::Texture &texture, Settings &set,
			       const std::vector<APanelScreen *> &panels)
{
  _content->construct(texture, set, panels);
  _leftSide->construct(texture, set, panels);
  _rightSide->construct(texture, set, panels);
  addPanels({_leftSide, _rightSide});
  resizeWidgets({std::stof(set.getCvarList().getCvar("r_width")),
	std::stof(set.getCvarList().getCvar("r_height"))});
}

void		SwitchPanel::draw(sf::RenderWindow &window, bool toWin)
{
  sf::Sprite	tmp;
  sf::IntRect	tmpZone;
  const std::vector<AWidget *> &content = _content->getWidgets();

  _rt.clear(sf::Color(127,127,127,0));
  if (!content.empty())
      content[_idx]->draw(_rt);
  for (auto &widget : _widgets)
    if (!widget->isHidden())
      widget->draw(_rt);
  for (auto &panel : _panels)
    if (!panel->isHidden())
      panel->draw(window, false);
  _rt.display();
  if (toWin)
    {
      print(_rt);
      _rt.display();
      tmp.setPosition(_zone.left, _zone.top);
      tmp.setTexture(_rt.getTexture());
      tmp.setTextureRect(static_cast<sf::IntRect>(_zone));
      window.draw(tmp);
    }
}

int	SwitchPanel::update(const sf::Event &event, sf::RenderWindow &ref,
			    Settings &set)
{
  const std::vector<AWidget *> &contentWidgets = _content->getWidgets();
  int	retVal = 0;
  int	dir;

  if (!contentWidgets.empty())
    retVal = contentWidgets[_idx]->update(event, ref, set);
  if (!retVal && isOver(ref) && isClicked(event, sf::Mouse::Left))
    {
      retVal = 1;
      if ((dir = isOverContent(ref)) != 0)
	switchContent(dir);
    }
  return retVal;
}

void	SwitchPanel::addContent(AWidget * const widget)
{
  _content->addWidget(widget);
}

void	SwitchPanel::addContent(const std::initializer_list<AWidget * const> &widgets)
{
  _content->addWidget(widgets);
}

void	SwitchPanel::addSides(const std::array<AWidget * const, 2> &sides)
{
  _leftSide->addWidget(sides[0]);
  _rightSide->addWidget(sides[1]);
}

void		SwitchPanel::switchContent(int dir)
{
  unsigned int	contentSize;

  contentSize = getContentSize();
  if (contentSize == 0)
    return ;
  _idx = ((dir == 1) ? ((_idx + 1) % contentSize) : ((contentSize + _idx - 1) % contentSize));
}

void	SwitchPanel::trigger(const t_event &event)
{
  if (event.e & wEvent::Update)
    switchContent(event.idx);
  else
    APanelScreen::trigger(event);
}

unsigned int	SwitchPanel::getContentSize() const
{
  return _content->getWidgets().size();
}

const sf::FloatRect	&SwitchPanel::getLeftZone() const
{
  return _leftSide->getZone();
}

const sf::FloatRect	&SwitchPanel::getRightZone() const
{
  return _rightSide->getZone();
}

const sf::FloatRect	&SwitchPanel::getContentZone() const
{
  return _content->getZone();
}

bool	SwitchPanel::isOver(const sf::RenderWindow &ref) const
{
  return _zone.contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(ref)));
}

int	SwitchPanel::isOverContent(const sf::RenderWindow &ref) const
{
  const std::vector<APanelScreen *>	&vec = getSubPanels();

  if (vec[0]->getZone().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(ref))))
    return -1;
  else if (vec[1]->getZone().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(ref))))
    return 1;
  return 0;
}

bool	SwitchPanel::isClicked(const sf::Event &event, sf::Mouse::Button button) const
{
  if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == button)
	return true;
    }
  return false;
}
