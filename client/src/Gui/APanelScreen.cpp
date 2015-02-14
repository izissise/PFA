#include "APanelScreen.hpp"
#include "Panel.hpp"

APanelScreen::APanelScreen(const sf::FloatRect &zone) :
  _hide(false),
  _zone(zone),
  _state(APanelScreen::State::None),
  _flag(APanelScreen::Display::None)
{
}

void         APanelScreen::draw(sf::RenderTarget &window, bool first)
{
  sf::RenderTarget &target = (_flag & APanelScreen::Display::Overlap ? _rt : window);
  // here i get the target to draw into

  if (&target != &window)                       // then we get a new RenderTarget
    target.clear(sf::Color(127,127,127,0));     // clear it before any usage
  if (dynamic_cast<sf::RenderTexture *>(&target) != nullptr) // draw content into the renderTexture
    {
      for (auto &widget : _widgets)
        if (!widget->isHidden())
          widget->draw(target);
    }
  for (auto &panel : _panels)
    if (!panel->isHidden())
      panel->draw(target, false);
  if (first)
    print(window, false);
}

void    APanelScreen::print(sf::RenderTarget &window, bool isTextured)
{
  isTextured = (isTextured || _flag & APanelScreen::Display::Overlap);
  if (!isTextured)
    {
      for (auto widget : _widgets)
        if (!widget->isHidden())
          widget->draw(window);
    }
  else if (_flag & APanelScreen::Display::Overlap) // means it is the main RenderTexture
    {
      _rt.display();            // Must refresh before drawing
      sf::Sprite                sprite(_rt.getTexture(), static_cast<sf::IntRect>(_zone));

      sprite.setPosition(_zone.left, _zone.top);
      window.draw(sprite);
    }
  for (auto panel : _panels)
    if (!panel->isHidden())
      panel->print(window, isTextured);
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

void    APanelScreen::setDisplayFlag(APanelScreen::Display flag)
{
  _flag = flag;
}

auto    APanelScreen::getDisplayFlag() const -> APanelScreen::Display
{
  return _flag;
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

bool    APanelScreen::checkPanelBounds(APanelScreen * const panel) const
{
  sf::FloatRect wZone = panel->getZone();

  return (wZone.left + wZone.width > _zone.left
          && wZone.top + wZone.height > _zone.top
          && wZone.left < _zone.left + _zone.width
          && wZone.top < _zone.top + _zone.height);
}

void    APanelScreen::setState(APanelScreen::State state)
{
  if (state & APanelScreen::State::Inactive)
    _countdown.start(sf::milliseconds(InactiveTime));
  _state = static_cast<APanelScreen::State>(_state | state);
}

void    APanelScreen::removeState(APanelScreen::State state)
{
  _state = static_cast<APanelScreen::State>(_state & static_cast<APanelScreen::State>(~(state)));
}

void    APanelScreen::moveWidgets(APanelScreen * const pan,
                                  const sf::Vector2f &displacement)
{
  auto  vec = pan->getWidgets();

  for (auto &it : vec)
    it->move(displacement.x, displacement.y);
}

void    APanelScreen::moveZone(const sf::Vector2f &displacement)
{
  _zone.top += displacement.y;
  _zone.left += displacement.x;
}

void    APanelScreen::move(const sf::Vector2f &displacement)
{
  for (auto &pit : _panels)
    {
      if (!pit->getSubPanels().empty())
        pit->move(displacement);
      else
        pit->moveWidgets(pit, displacement);
    }
  moveWidgets(this, displacement);
  moveZone(displacement);
}

void    APanelScreen::setPosition(const sf::Vector2f &position)
{
  sf::Vector2f  displacement(position.x - _zone.left,
                             position.y - _zone.top);

  move(displacement);
}

APanelScreen::State     APanelScreen::getState() const
{
  return _state;
}

int	APanelScreen::event(const sf::Event &ev, sf::RenderWindow &ref, Settings &set)
{
  int	retVal = 0;
    bool        overlap = _flag & APanelScreen::Display::Overlap;

  if (_state & APanelScreen::State::Inactive)
    {
      if (_countdown.update() == false)
        return 0;
      else
        removeState(APanelScreen::State::Inactive);
    }

  for (auto rit = _panels.rbegin(); rit != _panels.rend(); ++rit)
    {
     if (!(*rit)->isHidden())
        {
          if (!(overlap) || (overlap && checkPanelBounds(*rit)))
            {
              if ((retVal = (*rit)->event(ev, ref, set)) != 0)
                return retVal;
              else if ((*rit)->getState() == APanelScreen::State::Leader)
                return 1;
            }
        }
    }
  for (auto rrrit = _widgets.rbegin(); rrrit != _widgets.rend(); ++rrrit)
    {
     if (checkPanelBounds(*rrrit)) // update widget even if hidden
        if ((retVal = (*rrrit)->update(ev, ref, set)) != 0)
          return retVal;
    }
  return retVal;
}

void	APanelScreen::setTrigger(const std::function<void (const t_event &event)> &func)
{
  _trigger = func;
}

Panel   *APanelScreen::encapsulate(AWidget *widget) const
{
  sf::FloatRect zone = widget->getZone();
  Panel         *panel = new Panel(zone);

  panel->addWidget(widget);
  return panel;
}

void    APanelScreen::addPanel(APanelScreen * const panel)
{
  _panels.push_back(panel);
}

void	APanelScreen::addPanel(const std::initializer_list<APanelScreen * const>  &panels)
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
  else if (ev.e & wEvent::Show)
    {
      if (ev.e & wEvent::Toggle)
        _hide = !_hide;
      else
        _hide = false;
    }
  if (ev.e & wEvent::Reset)
    {
      t_event   evt = ev;

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

const AWidget   *APanelScreen::getWidget(const std::string &id) const
{
  auto it = std::find_if(_widgets.begin(), _widgets.end(), [&id](AWidget *widget)
                         {
                           return (widget->getId() == id);
                         });
  if (it == _widgets.end())
    throw (std::runtime_error(id + " not found"));
  return (*it);
}

AWidget         *APanelScreen::getWidget(const std::string &id)
{
  auto it = std::find_if(_widgets.begin(), _widgets.end(), [&id](AWidget *widget)
                         {
                           return (widget->getId() == id);
                         });
  if (it == _widgets.end())
    throw (std::runtime_error(id + " not found"));
  return (*it);
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
