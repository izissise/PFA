#include "ScrollWidget.hpp"

ScrollWidget::ScrollWidget(const std::string &id, const sf::FloatRect &zone,
			   Scroll dir, const sf::Text &text) :
  AWidget(id, zone, text), _active(false), _dir(dir)
{
}

void		ScrollWidget::movePicker(sf::Sprite &sprite, float x, float y)
{
  sf::FloatRect	barRect = getSprite(0).sprite.getGlobalBounds();
  sf::FloatRect pickerRect = sprite.getGlobalBounds();

  if (_dir == Scroll::Horizontal)
    {
      x = (x > (barRect.left + barRect.width) ? (barRect.left + barRect.width) :
	   x < (barRect.left) ? barRect.left : x);
      x -= pickerRect.width / 2.0;
      y = barRect.top + barRect.height / 2.0 - pickerRect.height / 2.0;
    }
  else
    {
      y = (y > (barRect.top + barRect.height - pickerRect.height) ?
	   (barRect.top + barRect.height - pickerRect.height) :
       	   y < (barRect.top) ? (barRect.top) : y);
      x = barRect.left + barRect.width / 2.0 - pickerRect.width / 2.0;
    }
  sprite.setPosition(x, y);
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
	  t_sprite	&sprite = getSprite(1);
	  sf::Vector2f	pos = sprite.sprite.getPosition();

	  if (_dir == Scroll::Horizontal)
	    movePicker(sprite.sprite, event.mouseButton.x, pos.y);
	  else
	    movePicker(sprite.sprite, pos.x, event.mouseButton.y);
	}
    }
  else if (event.type == sf::Event::MouseMoved
	   && _active
	   && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      t_sprite	&sprite = getSprite(1);
      sf::Vector2f	pos = sprite.sprite.getPosition();

      if (_dir == Scroll::Horizontal)
	movePicker(sprite.sprite, event.mouseMove.x, pos.y);
      else
	movePicker(sprite.sprite, pos.x, event.mouseMove.y);
    }
  for (auto &func : _updates)
    {
      if ((retVal = func.second(*this, event, ref)) != 0)
	return retVal;
    }
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
  if (_dir == Scroll::Vertical && _sprites.empty())
    elem.sprite.rotate(90);
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
