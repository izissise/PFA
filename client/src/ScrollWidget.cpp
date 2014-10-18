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
      x = ((x + pickerRect.left + pickerRect.width) > (barRect.left + barRect.width) ?
	   (barRect.left + barRect.width - pickerRect.width) :
	   (x + pickerRect.left) < (barRect.left) ? (barRect.left) : x + pickerRect.left);
      y = barRect.top + barRect.height / 2.0 - pickerRect.height / 2.0;
    }
  else
    {
      y = ((y + pickerRect.top + pickerRect.height) > (barRect.top + barRect.height) ?
	   (barRect.top + barRect.height - pickerRect.height) :
       	   (y + pickerRect.top) < (barRect.top) ? (barRect.top) : y + pickerRect.top);
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

int		ScrollWidget::handleMouse(int pX, int pY)
{
  int		retVal = 0;
  sf::Sprite	&sprite = getSprite(1).sprite;

  if (_active)
    {
      if (_dir == Scroll::Horizontal)
	movePicker(sprite, pX - _mousePos.x, 0);
      else
	movePicker(sprite, 0, pY - _mousePos.y);
      retVal = 1;
    }
  _mousePos = {pX, pY};
  return retVal;
}

int	ScrollWidget::update(const sf::Event &event, sf::RenderWindow &ref,
			     Settings &set UNUSED)
{
  int	retVal = 0;

  if (isClicked(event, sf::Mouse::Left))
    {
      _active = isOver(ref);
      retVal = _active;
      if (_active)
	{
	  if (!isOver(ref, 1))
	    {
	      sf::FloatRect	picZone = getSprite(1).sprite.getGlobalBounds();
	      _mousePos = {static_cast<int>(picZone.left + picZone.width / 2),
			   static_cast<int>(picZone.top + picZone.height / 2)};
	      handleMouse(event.mouseButton.x, event.mouseButton.y);
	    }
	  else
	    _mousePos = sf::Mouse::getPosition(ref);
	}
    }
  else if (event.type == sf::Event::MouseMoved
	   && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    retVal = handleMouse(event.mouseMove.x, event.mouseMove.y);
  else if (event.type == sf::Event::MouseWheelMoved)
    {
      sf::Sprite	&sprite = getSprite(1).sprite;

      movePicker(sprite, 0, -SLIDESPEED * event.mouseWheel.delta);
      retVal = 1;
    }
  if (_update)
    {
      if (_update(*this, event, ref) != 0)
	retVal = 1;
    }
  updateScrollSize();
  return retVal;
}

bool	ScrollWidget::isOver(const sf::RenderWindow &ref, unsigned int spritePos) const
{
  return _sprites[spritePos].sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>
							       (sf::Mouse::getPosition(ref)));
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
    movePicker(elem.sprite, 0, 0);
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
    movePicker(sprite, 0, 0);
}
