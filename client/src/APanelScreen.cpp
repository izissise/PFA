#include "APanelScreen.hpp"

APanelScreen::APanelScreen(const sf::FloatRect &zone) :
  _hide(false), _zone(zone)
{
}

void		APanelScreen::draw(sf::RenderWindow &window, bool toWin)
{
  sf::Sprite	tmp;
  sf::IntRect	tmpZone;

  _rt.clear(sf::Color(127,127,127,0));
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

void	APanelScreen::print(sf::RenderTexture &rt) const
{
  sf::Sprite	tmp;
  sf::IntRect	tmpZone;

  for (auto &panel : _panels)
    if (!panel->isHidden())
      {
	tmpZone = static_cast<sf::IntRect>(panel->getZone());
	tmp.setPosition(tmpZone.left, tmpZone.top);
	tmp.setTexture(panel->getRT().getTexture());
	tmp.setTextureRect(tmpZone);
	rt.draw(tmp);
	panel->print(rt);
      }
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
  float	ratioX = size.x / SIZEX;
  float	ratioY = size.y / SIZEY;

  _rt.create(size.x, size.y);
  _zone.left *= ratioX;
  _zone.top *= ratioY;
  _zone.width *= ratioX;
  _zone.height *= ratioY;
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

bool	APanelScreen::checkPanelBounds(AWidget * const widget) const
{
  sf::FloatRect	wZone = widget->getZone();

  return (wZone.left + wZone.width > _zone.left
	  && wZone.top + wZone.height > _zone.top
	  && wZone.left < _zone.left + _zone.width
	  && wZone.top < _zone.top + _zone.height);
}

int	APanelScreen::event(const sf::Event &ev, sf::RenderWindow &ref, Settings &set)
{
  int	retVal = 0;

  for (auto rit = _panels.rbegin(); rit != _panels.rend(); ++rit)
    {
      if ((*rit)->isHidden() == false)
	if ((retVal = (*rit)->event(ev, ref, set)) != 0)
	  return retVal;
    }
  for (auto rit = _widgets.rbegin(); rit != _widgets.rend(); ++rit)
    {
      if (checkPanelBounds(*rit))
	if ((retVal = (*rit)->update(ev, ref, set)) != 0)
	  return retVal;
    }
  return retVal;
}

void	APanelScreen::setTrigger(const std::function<void (const t_event &event)> &func)
{
  _trigger = func;
}

void	APanelScreen::addPanels(const std::initializer_list<APanelScreen * const>  &panels)
{
  for (auto &panel : panels)
    _panels.push_back(panel);
}

void	APanelScreen::trigger(const t_event &ev)
{
  if (ev.e & wEvent::Hide)
    {
      if (ev.e & wEvent::Toggle)
	_hide = !_hide;
      else
	_hide = true;
    }
}

void	APanelScreen::addWidget(AWidget * const widget)
{
  _widgets.push_back(widget);
}

void	APanelScreen::addWidget(const std::initializer_list<AWidget * const> &widgets)
{
  for (auto widget : widgets)
    _widgets.push_back(widget);
}

void		APanelScreen::addFont(const std::string &fontName,
				      const std::string &fontPath)
{
  sf::Font	font;

  if (!font.loadFromFile(fontPath))
    throw (std::invalid_argument(fontPath + " does not exist"));
  _font.insert(std::pair<std::string, sf::Font>(fontName, font));
}

const std::vector<AWidget *>	&APanelScreen::getWidgets() const
{
  return _widgets;
}

const std::vector<APanelScreen *>	&APanelScreen::getSubPanels() const
{
  return _panels;
}

std::vector<APanelScreen *>	&APanelScreen::getSubPanels()
{
  return _panels;
}

const sf::RenderTexture	&APanelScreen::getRT() const
{
  return _rt;
}

const sf::FloatRect	&APanelScreen::getZone() const
{
  return _zone;
}
