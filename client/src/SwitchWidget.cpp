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

void		SwitchWidget::draw(sf::RenderWindow &window, bool toWin)
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
