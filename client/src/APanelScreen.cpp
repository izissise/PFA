#include "APanelScreen.hpp"

APanelScreen::APanelScreen() :
  _hide(false)
{
}

APanelScreen::~APanelScreen()
{
}

void		APanelScreen::draw(sf::RenderWindow &window)
{
  if (_hide)
    return ;
  for (auto &panel : _panels)
    panel->draw(window);
  for (auto &widget : _widgets)
    widget->draw(window);
}

sf::Vector2f	APanelScreen::toPixel(const sf::Vector2f &perCent,
				      const sf::Vector2f &size) const
{
  return sf::Vector2f(perCent.x * size.x, perCent.y * size.y);
}

sf::FloatRect	APanelScreen::toPixel(const sf::Vector2f &perCent,
				      const sf::FloatRect &rect) const
{
  return sf::FloatRect(rect.left * perCent.x, rect.top * perCent.y,
		       rect.width * perCent.x, rect.height * perCent.y);
}

void		APanelScreen::resizeWidgets(const sf::Vector2f &size)
{
  for (auto &elem : _widgets)
    elem->scale(size);
}

void		APanelScreen::saveTexture(sf::Texture * const texture)
{
  if (std::find(_textures.begin(), _textures.end(), texture) == _textures.end())
    _textures.push_back(texture);
}

const sf::Texture	*APanelScreen::addSpriteForWidget(AWidget * const widget,
							  const sf::Color &color,
							  const sf::Vector2f &size,
							  bool display)
{
  sf::Image	image;
  sf::Texture	*texture = new sf::Texture;
  t_sprite	elem(sf::Sprite(), display);

  image.create(size.x, size.y, color);
  texture->loadFromImage(image);
  elem.sprite.setTexture(*texture);
  saveTexture(texture);
  widget->addSprite(elem);
  return (_textures.back());
}

bool	APanelScreen::isHidden() const
{
  return _hide;
}

void	APanelScreen::setHide(bool hide)
{
  _hide = hide;
}

int	APanelScreen::run(const sf::Event &event, sf::RenderWindow &ref, Settings &set)
{
  int	retVal = 0;

  for (auto rit = _panels.rbegin(); rit != _panels.rend(); ++rit)
    {
      if ((*rit)->isHidden() == false)
	if ((retVal = (*rit)->run(event, ref, set)) != 0)
	  return retVal;
    }
  for (auto rit = _widgets.rbegin(); rit != _widgets.rend(); ++rit)
    {
      if ((retVal = (*rit)->update(event, ref, set)) != 0)
	return retVal;
    }
  return retVal;
}

void	APanelScreen::addPanels(const std::vector<APanelScreen *> &panels)
{
  for (auto &panel : panels)
    _panels.push_back(panel);
}

void	APanelScreen::trigger(const t_event &event)
{
  if (event.e & wEvent::Hide)
    {
      if (event.e & wEvent::Toggle)
	_hide = !_hide;
      else
	{
	  _hide = event.value;
	}
    }
}

const std::vector<AWidget *>	&APanelScreen::getWidgets() const
{
  return _widgets;
}
