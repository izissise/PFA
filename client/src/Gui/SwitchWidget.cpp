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

void		SwitchWidget::print(sf::RenderTarget &window, bool isTextured)
{
  const std::vector<AWidget *> &content = _content->getWidgets();
  // here i get the target to draw into

  isTextured = (isTextured || _flag & APanelScreen::Display::Overlap);
  if (!isTextured)
    {
      if (!content.empty())
	content.at(_idx)->draw(window);
    }
  else if (_flag & APanelScreen::Display::Overlap) // means it is the main RenderTexture
    {
      _rt.display();		// Must refresh before drawing
      sf::Sprite		sprite(_rt.getTexture(), static_cast<sf::IntRect>(_zone));

      sprite.setPosition(_zone.left, _zone.top);
      window.draw(sprite);
    }
  for (auto panel : _panels)
    if (!panel->isHidden())
      panel->print(window, isTextured);
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
