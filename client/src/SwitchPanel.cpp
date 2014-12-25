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

void	SwitchPanel::print(sf::RenderTarget &window, bool isTextured)
{
  const std::vector<APanelScreen *> &content = _content->getSubPanels();
  // here i get the target to draw into

  isTextured = (isTextured || _flag & APanelScreen::Display::Overlap);
  if (!isTextured)
    {
      for (auto widget : _widgets)
	if (!widget->isHidden())
	  widget->draw(window);
    }
  else if (_flag & APanelScreen::Display::Overlap) // means it is the main RenderTexture
    {
      _rt.display();		// Must refresh before drawing
      sf::Sprite		sprite(_rt.getTexture(), static_cast<sf::IntRect>(_zone));

      sprite.setPosition(_zone.left, _zone.top);
      window.draw(sprite);
    }
  if (!content.empty())
    content.at(_idx)->print(window, isTextured);
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
