#include "ScrollWidget.hpp"

ScrollWidget::ScrollWidget(const std::string &id, const sf::FloatRect &zone,
			   Scroll dir, APanelScreen *panel,
			   const sf::Text &text, wFlag flg) :
  AWidget(id, zone, text, flg), _active(false), _dir(dir), _panel(panel), _ratio(1)
{
}

void		ScrollWidget::moveWidgets(APanelScreen * const pan,
					  const sf::Vector2f &moveSize)
{
  auto		vec = pan->getWidgets();

  for (auto &it : vec)
    {
      if (it->getFlag() & wFlag::Movable)
      	it->move(-moveSize.x, -moveSize.y);
    }
}

void		ScrollWidget::moveSubPanels(APanelScreen * const pan,
					    const sf::Vector2f &moveSize)
{
  auto		panels = pan->getSubPanels();

  for (auto &pit : panels)
    {
      if (!pit->getSubPanels().empty())
	moveSubPanels(pit, moveSize);
      else
	moveWidgets(pit, moveSize);
    }
  moveWidgets(pan, moveSize);
}

void		ScrollWidget::movePicker(sf::Sprite &sprite, float x, float y)
{
  sf::FloatRect	barRect = getSprite(0).sprite.getGlobalBounds();
  sf::FloatRect pickerRect = sprite.getGlobalBounds();
  sf::Vector2f	spritePos = sprite.getPosition();
  sf::Vector2f	moveSize;

  if (_dir == Scroll::Horizontal)
    {
      x = ((x + pickerRect.width) > (barRect.left + barRect.width) ?
	   (barRect.left + barRect.width - pickerRect.width) :
	   x < (barRect.left) ? (barRect.left) : x);
      y = barRect.top + barRect.height / 2.0 - pickerRect.height / 2.0;
    }
  else
    {
      y = ((y + pickerRect.height) > (barRect.top + barRect.height) ?
	   (barRect.top + barRect.height - pickerRect.height) :
       	   y < (barRect.top) ? (barRect.top) : y);
      x = barRect.left + barRect.width / 2.0 - pickerRect.width / 2.0;
    }
  sprite.setPosition(x, y);
  moveSize = sf::Vector2f(x, y) - spritePos;
  moveSize.y *= _ratio;
  moveSize.x *= _ratio;
  if (_dir == Scroll::Horizontal)
    moveSize.y = 0;
  else
    moveSize.x = 0;
  moveSubPanels(_panel, moveSize);
}

void		ScrollWidget::updateButtonPos()
{
  if (_sprites.size() < 3)
    return ;

  sf::FloatRect	picZone = getSprite(1).sprite.getGlobalBounds();
  t_sprite	&button = getSprite(2);
  sf::FloatRect	buttonZone = button.sprite.getGlobalBounds();

  if (_dir == Scroll::Vertical)
    {
      button.draw = (picZone.height >= buttonZone.height);
      button.sprite.setPosition(picZone.left, picZone.top + picZone.height / 2.0
				- buttonZone.height / 2.0);
    }
  else
    {
      button.draw = (picZone.width >= buttonZone.width);
      button.sprite.setPosition(picZone.left + picZone.width / 2.0 - buttonZone.width / 2.0,
				picZone.top);
    }
}

unsigned int	ScrollWidget::getBiggest(const sf::FloatRect &barZone,
					 float diff,
					 const std::vector<AWidget *> &widgetList)
{
  unsigned int	biggest = 0;
  sf::FloatRect	zone;

  for (auto &it : widgetList)
    {
      if (it->isHidden())
	continue ;
      zone = it->getZone();
      if (_dir == Scroll::Vertical)
	{
	  if (zone.top + zone.height - barZone.top + diff > biggest)
	    biggest = zone.top + zone.height - barZone.top + diff;
	}
      else
	{
	  if (zone.left + zone.width - barZone.left + diff > biggest)
	    biggest = zone.left + zone.width - barZone.left + diff;
	}
    }
  return biggest;
}

unsigned int	ScrollWidget::calcPanelSize(APanelScreen * const pan, float diff,
					    const sf::FloatRect &barZone)
{
  unsigned int	tmp;
  unsigned int	biggest = 0;

  for (auto &panel : pan->getSubPanels())
    {
      if (panel->isHidden())
	continue ;
      if (!panel->getSubPanels().empty())
	{
	  tmp = calcPanelSize(panel, diff, barZone);
	  biggest = BIGGEST(biggest, tmp);
	}
      else
	{
	  tmp = getBiggest(barZone, diff, panel->getWidgets());
	  biggest = BIGGEST(biggest, tmp);
	}
    }
  tmp = getBiggest(barZone, diff, pan->getWidgets());
  biggest = BIGGEST(biggest, tmp);
  return biggest;
}

void		ScrollWidget::updateScrollSize()
{
  sf::FloatRect	barZone = getSprite(0).sprite.getGlobalBounds();
  sf::FloatRect	picZone = getSprite(1).sprite.getGlobalBounds();
  unsigned int	biggest;
  float		diff;

  biggest = barZone.width;
  if (_dir == Scroll::Vertical)
    diff = (picZone.top - barZone.top) * _ratio;
  else
    diff = (picZone.left - barZone.left) * _ratio;
  biggest = calcPanelSize(_panel, diff, barZone);
  if (biggest <= (_dir == Scroll::Vertical ? barZone.height : barZone.width))
    _ratio = 1;
  else
    _ratio = biggest / (_dir == Scroll::Vertical ? barZone.height : barZone.width);
  toSize(1, _dir == Scroll::Vertical ? -1 : barZone.width / _ratio,
	 _dir == Scroll::Vertical ? barZone.height / _ratio : -1);
  updateButtonPos();
}

int		ScrollWidget::handleMouse(float pX, float pY)
{
  int		retVal = 0;
  sf::Sprite	&sprite = getSprite(1).sprite;
  sf::Vector2f	pos = sprite.getPosition();

  if (_active)
    {
      if (_dir == Scroll::Horizontal)
	movePicker(sprite, pX, pos.y);
      else
	movePicker(sprite, pos.x, pY);
      retVal = 1;
    }
  return retVal;
}

int	ScrollWidget::update(const sf::Event &event, sf::RenderWindow &ref,
			     Settings &set UNUSED)
{
  int	retVal = 0;

  if (_hide)
    return 0;
  if (isClicked(event, sf::Mouse::Left))
    {
      _active = isOver(ref);
      retVal = handleMouse(event.mouseButton.x, event.mouseButton.y);
    }
  else if (event.type == sf::Event::MouseMoved
	   && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    retVal = handleMouse(event.mouseMove.x, event.mouseMove.y);
  else if (event.type == sf::Event::MouseWheelMoved)
    {
      sf::Sprite	&sprite = getSprite(1).sprite;
      sf::Vector2f	pos = sprite.getPosition();

      movePicker(sprite, pos.x, pos.y - SLIDESPEED * event.mouseWheel.delta);
      retVal = 1;
    }
  for (auto &func : _updates)
    {
      if (func.second(*this, event, ref) != 0)
	{
	  retVal = 1;
	  break ;
	}
    }
  updateScrollSize();
  return retVal;
}

bool	ScrollWidget::isOver(const sf::RenderWindow &ref) const
{
  for (auto &elem : _sprites)
    {
      if (elem.sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>
						 (sf::Mouse::getPosition(ref))))
	return true;
    }
  return false;
}

void	ScrollWidget::addSprite(t_sprite &elem)
{
  if (_sprites.size() == 1)
    {
      sf::Vector2f	pos = elem.sprite.getPosition();

      movePicker(elem.sprite, pos.x, pos.y);
    }
  else
    elem.sprite.setPosition(_zone.left, _zone.top);
  _sprites.push_back(elem);
}

void	ScrollWidget::addSprite(const sf::Texture &texture, const sf::IntRect &rect,
				bool display)
{
  t_sprite	sprite(sf::Sprite(texture, rect), display);

  addSprite(sprite);
}

void	ScrollWidget::toSize(unsigned int spritePos, float pX, float pY)
{
  sf::Sprite	&sprite = getSprite(spritePos).sprite;
  sf::FloatRect rec = sprite.getGlobalBounds();
  float		rX = (pX == -1 ? 1 : (pX / rec.width));
  float		rY = (pY == -1 ? 1 : (pY / rec.height));

  sprite.scale(rX, rY);
  if (spritePos == 1)
    {
      sf::Vector2f	pos = sprite.getPosition();

      movePicker(sprite, pos.x, pos.y);
    }
}
