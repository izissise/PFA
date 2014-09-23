#include "ScrollWidget.hpp"

ScrollWidget::ScrollWidget(const std::string &id, const sf::FloatRect &zone,
			   Scroll dir, APanelScreen *panel,
			   const sf::Text &text, wFlag flg) :
  AWidget(id, zone, text, flg), _active(false), _dir(dir), _panel(panel)
{
}

void		ScrollWidget::moveWidgets(sf::Vector2f moveSize)
{
  auto		vec = _panel->getWidgets();

  if (_dir == Scroll::Horizontal)
    moveSize.y = 0;
  else
    moveSize.x = 0;
  for (auto &it : vec)
    {
      if (it->getFlag() & wFlag::Movable)
      	it->move(-moveSize.x, -moveSize.y);
    }
}

void		ScrollWidget::movePicker(sf::Sprite &sprite, float x, float y)
{
  sf::FloatRect	barRect = getSprite(0).sprite.getGlobalBounds();
  sf::FloatRect pickerRect = sprite.getGlobalBounds();
  sf::Vector2f	spritePos = sprite.getPosition();

  if (_dir == Scroll::Horizontal)
    {
      x = (x > (barRect.left + barRect.width) ? (barRect.left + barRect.width) :
	   x < (barRect.left) ? barRect.left : x);
      x -= pickerRect.width / 2.0;
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
  moveWidgets(sf::Vector2f(x, y) - spritePos);
}

void		ScrollWidget::updateScrollSize()
{
  auto		vec = _panel->getWidgets();
  sf::FloatRect	zone;
  sf::FloatRect selfZone;
  sf::FloatRect	barZone;
  unsigned int	biggest;
  sf::Sprite	&sprite = getSprite(1).sprite;
  sf::Vector2f	scaleVec = sprite.getScale();
  float		diff;

  barZone = sprite.getGlobalBounds();
  selfZone = _panel->getWidgets().front()->getZone();
  biggest = selfZone.height;
  diff = barZone.top - selfZone.top;
  for (auto &it : vec)
    {
      zone = it->getZone();
      std::cout << zone.top + zone.height - selfZone.top << std::endl;
      if (zone.top + zone.height - selfZone.top + diff > biggest)
	biggest = zone.top + zone.height - selfZone.top + diff;
    }
  biggest = selfZone.height - (biggest - selfZone.height);
  toSize(1, -1, biggest);
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
      if (_active)
	{
	  sf::Sprite	&sprite = getSprite(1).sprite;
	  sf::Vector2f	pos = sprite.getPosition();

	  if (_dir == Scroll::Horizontal)
	    movePicker(sprite, event.mouseButton.x, pos.y);
	  else
	    movePicker(sprite, pos.x, event.mouseButton.y);
	}
    }
  else if (event.type == sf::Event::MouseMoved
	   && _active
	   && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      sf::Sprite	&sprite = getSprite(1).sprite;
      sf::Vector2f	pos = sprite.getPosition();

      if (_dir == Scroll::Horizontal)
	movePicker(sprite, event.mouseMove.x, pos.y);
      else
	movePicker(sprite, pos.x, event.mouseMove.y);
    }
  for (auto &func : _updates)
    {
      if ((retVal = func.second(*this, event, ref)) != 0)
	return retVal;
    }
  updateScrollSize();
  return 0;
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
  rec = sprite.getGlobalBounds();
}
