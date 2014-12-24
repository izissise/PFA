#include "SwitchWidget.hpp"

SwitchWidget::SwitchWidget(const sf::FloatRect &panelZone,
			   const sf::FloatRect &contentZone) :
  ASwitchElem(panelZone, contentZone)
{
}

int	SwitchWidget::update(const sf::Event &event, sf::RenderWindow &ref,
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

void		SwitchWidget::draw(sf::RenderTarget &window, bool first)
{
  const std::vector<AWidget *> &content = _content->getWidgets();
  sf::RenderTarget &target = (_flag & APanelScreen::Display::Overlap ? _rt : window);
  // here i get the target to draw into

  if (&target != &window)			// then we get a new RenderTarget
    target.clear(sf::Color(127,127,127,0));	// clear it before any usage
  if (!content.empty())
    content.at(_idx)->draw(target);
}

void	SwitchWidget::addContent(AWidget * const widget)
{
  _content->addWidget(widget);
}

void	SwitchWidget::addContent(const std::initializer_list<AWidget * const> &widgets)
{
  _content->addWidget(widgets);
}

unsigned int	SwitchWidget::getContentSize() const
{
  return _content->getWidgets().size();
}
