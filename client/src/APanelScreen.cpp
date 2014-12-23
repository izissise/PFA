#include "APanelScreen.hpp"

APanelScreen::APanelScreen(const sf::FloatRect &zone) :
  _hide(false),
  _zone(zone),
  _state(APanelScreen::State::None),
  _flag(APanelScreen::Display::None)
{
}

APanelScreen::~APanelScreen()
{
}

void         APanelScreen::draw(sf::RenderTarget &window, bool first)
{
  sf::RenderTarget &target = (_flag & APanelScreen::Display::Overlap ? _rt : window);
  // here i get the target to draw into

  if (&target != &window)	 // then we get a new RenderTarget
    target.clear(sf::Color(127,127,127,0));	// clear it before any usage
  for (auto &widget : _widgets)
    if (!widget->isHidden())
      widget->draw(target);
  for (auto &panel : _panels)
    if (!panel->isHidden())
      panel->draw(target, false);
  if (first)
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

void	APanelScreen::print(sf::RenderWindow &window) const
{
  sf::Sprite	sprite;
  sf::IntRect	spriteZone;

  for (auto &panel : _panels)
    if (!panel->isHidden())
      {
	sf::RenderTarget *target;
	if (_flag & APanelScreen::Display::Overlap)
	  target = &panel->getRT();
	else
	  target = &window;
	if (target != &window) // then it's a renderTexture
	  {
	    sf::RenderTexture *rt = dynamic_cast<sf::RenderTexture *>(target);
	    rt->display(); // Must refresh before drawing
	    spriteZone = static_cast<sf::IntRect>(panel->getZone());
	    sprite.setPosition(spriteZone.left, spriteZone.top);
	    sprite.setTexture(rt->getTexture());
	    sprite.setTextureRect(spriteZone);
	    window.draw(sprite);
	  }
	panel->print(window);
      }
}

// void		APanelScreen::draw(sf::RenderWindow &window, bool toWin)
// {
//   sf::Sprite	tmp;
//   sf::IntRect	tmpZone;

//   _rt.clear(sf::Color(127,127,127,0));
//   for (auto &widget : _widgets)
//     if (!widget->isHidden())
//       widget->draw(_rt);
//   for (auto &panel : _panels)
//     if (!panel->isHidden())
//       panel->draw(window, false);
//   _rt.display();
//   if (toWin)
//     {
//       print(_rt);
//       _rt.display();
//       tmp.setPosition(_zone.left, _zone.top);
//       tmp.setTexture(_rt.getTexture());
//       tmp.setTextureRect(static_cast<sf::IntRect>(_zone));
//       window.draw(tmp);
//     }
// }

// void	APanelScreen::print(sf::RenderTexture &rt) const
// {
//   sf::Sprite	tmp;
//   sf::IntRect	tmpZone;

//   for (auto &panel : _panels)
//     if (!panel->isHidden())
//       {
// 	tmpZone = static_cast<sf::IntRect>(panel->getZone());
// 	tmp.setPosition(tmpZone.left, tmpZone.top);
// 	tmp.setTexture(panel->getRT().getTexture());
// 	tmp.setTextureRect(tmpZone);
// 	rt.draw(tmp);
// 	panel->print(rt);
//       }
// }

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

  if (_flag & APanelScreen::Display::Overlap)
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

void	APanelScreen::setDisplayFlag(APanelScreen::Display flag)
{
  _flag = flag;
}

void	APanelScreen::setHide(bool hide)
{
  _hide = hide;
}

void	APanelScreen::setState(APanelScreen::State state)
{
  _state = state;
}

void	APanelScreen::moveWidgets(APanelScreen * const pan,
				  const sf::Vector2f &displacement)
{
  auto	vec = pan->getWidgets();

  for (auto &it : vec)
    it->move(displacement.x, displacement.y);
}

void	APanelScreen::move(const sf::Vector2f &displacement)
{
  for (auto &pit : _panels)
    {
      if (!pit->getSubPanels().empty())
	pit->move(displacement);
      else
	pit->moveWidgets(pit, displacement);
    }
  moveWidgets(this, displacement);
  _zone.top += displacement.y;
  _zone.left += displacement.x;
}

void	APanelScreen::setPosition(const sf::Vector2f &position)
{
  sf::Vector2f	displacement(position.x - _zone.left,
			     position.y - _zone.top);

  move(displacement);
}

APanelScreen::State	APanelScreen::getState() const
{
  return _state;
}

bool	APanelScreen::checkPanelBounds(AWidget * const widget) const
{
  sf::FloatRect	wZone = widget->getZone();

  return (wZone.left + wZone.width > _zone.left
	  && wZone.top + wZone.height > _zone.top
	  && wZone.left < _zone.left + _zone.width
	  && wZone.top < _zone.top + _zone.height);
}

int	APanelScreen::update(const sf::Event &event, sf::RenderWindow &ref, Settings &set)
{
  int	retVal = 0;

  for (auto rit = _panels.rbegin(); rit != _panels.rend(); ++rit)
    {
      if ((*rit)->isHidden() == false)
	{
	  if ((retVal = (*rit)->update(event, ref, set)) != 0)
	    return retVal;
	  else if ((*rit)->getState() == APanelScreen::State::Leader)
	    return 1;
	}
    }
  for (auto rit = _widgets.rbegin(); rit != _widgets.rend(); ++rit)
    {
      if (checkPanelBounds(*rit))
	if ((retVal = (*rit)->update(event, ref, set)) != 0)
	  return retVal;
    }
  return retVal;
}

void	APanelScreen::setTrigger(const std::function<void (const t_event &event)> &func)
{
  _trigger = func;
}

void	APanelScreen::addPanel(APanelScreen * const panel)
{
  _panels.push_back(panel);
}

void	APanelScreen::addPanel(const std::initializer_list<APanelScreen * const>  &panels)
{
  for (auto &panel : panels)
    _panels.push_back(panel);
}

void		APanelScreen::trigger(const t_event &event)
{
  if (event.e & wEvent::Hide)
    {
      if (event.e & wEvent::Toggle)
	_hide = !_hide;
      else
	_hide = true;
    }
  if (event.e & wEvent::Reset)
    {
      t_event	evt = event;

      evt.e = static_cast<wEvent>(evt.e & wEvent::None) | wEvent::Reset;
      for (AWidget *widget : _widgets)
	widget->trigger(evt);
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

sf::RenderTexture	&APanelScreen::getRT()
{
  return _rt;
}

const sf::FloatRect	&APanelScreen::getZone() const
{
  return _zone;
}
