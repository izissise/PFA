#include "SwitchPanel.hpp"

SwitchPanel::SwitchPanel(const sf::FloatRect &panelZone,
			 const sf::FloatRect &contentZone) :
  ASwitchElem(panelZone, contentZone)
{
}

int	SwitchPanel::update(const sf::Event &event, sf::RenderWindow &ref,
			    Settings &set)
{
  const std::vector<APanelScreen *> &contentPanels = _content->getSubPanels();
  int	retVal = 0;
  int	dir;

  if (!contentPanels.empty())
    retVal = contentPanels[_idx]->update(event, ref, set);
  if (!retVal && isOver(ref) && isClicked(event, sf::Mouse::Left))
    {
      retVal = 1;
      if ((dir = isOverContent(ref)) != 0)
	switchContent(dir);
    }
  return retVal;
}

void	SwitchPanel::addContent(APanelScreen * const panel)
{
  _content->addPanel(panel);
}

void	SwitchPanel::addContent(const std::initializer_list<APanelScreen * const> &panels)
{
  _content->addPanel(panels);
}

unsigned int	SwitchPanel::getContentSize() const
{
  return _content->getSubPanels().size();
}
