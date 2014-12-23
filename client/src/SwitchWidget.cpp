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

void		SwitchWidget::draw(sf::RenderTarget &window, bool toWin)
{
  const std::vector<AWidget *> &content = _content->getWidgets();
  sf::RenderTarget &target = (_flag & APanelScreen::Display::Overlap ? _rt : window);
  // here i get the target to draw into

  if (&target != &window)	 // then we get a new RenderTarget
    target.clear(sf::Color(127,127,127,0));	// clear it before any usage
  if (!content.empty())
      content[_idx]->draw(target);
  for (auto &widget : _widgets)
    if (!widget->isHidden())
      widget->draw(target);
  for (auto &panel : _panels)
    if (!panel->isHidden())
      panel->draw(target, false);
  if (toWin)
    {
      if (&target != &window) // need to draw the renderTexture
	{
	  sf::Sprite	sprite;

	  dynamic_cast<sf::RenderTexture *>(&target)->display(); // Must refresh before drawing
	  sprite.setPosition(_zone.left, _zone.top);
	  sprite.setTexture(_rt.getTexture());
	  sprite.setTextureRect(static_cast<sf::IntRect>(_zone));
	  window.draw(sprite);
	}
      print(*dynamic_cast<sf::RenderWindow *>(&window));
    }
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
