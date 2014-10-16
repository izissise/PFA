#include "BarWidget.hpp"
#include "Unused.hpp"

BarWidget::BarWidget(const std::string &id, const sf::FloatRect &zone,
		     const sf::Text &text) :
  AWidget(id, zone, text), _active(false)
{
}

void		BarWidget::movePicker(sf::Sprite &sprite, float x, float y)
{
  sf::FloatRect	barRect = getSprite(0).sprite.getGlobalBounds();
  sf::FloatRect pickerRect = sprite.getGlobalBounds();

  x = (x > (barRect.left + barRect.width) ? (barRect.left + barRect.width) :
       x < (barRect.left) ? barRect.left : x);
  // y = (y > (barRect.top + barRect.height) ? (barRect.top + barRect.height) :
  //      y < (barRect.top) ? barRect.top : y);
  x -= pickerRect.width / 2.0;
  y = barRect.top + barRect.height / 2.0 - pickerRect.height / 2.0;
  sprite.setPosition(x, y);
}

int	BarWidget::update(const sf::Event &event, sf::RenderWindow &ref,
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

	  retVal = 1;
	  movePicker(sprite.sprite, event.mouseButton.x, pos.y);
	}
    }
  else if (event.type == sf::Event::MouseMoved
	   && _active
	   && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      t_sprite	&sprite = getSprite(1);
      sf::Vector2f	pos = sprite.sprite.getPosition();

      retVal = 1;
      movePicker(sprite.sprite, event.mouseMove.x, pos.y);
    }
  if (_update)
    {
      if (_update(*this, event, ref) != 0)
	return 1;
    }
  return retVal;
}

bool	BarWidget::isOver(const sf::RenderWindow &ref) const
{
  for (auto &elem : _sprites)
    {
      if (elem.sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>
						 (sf::Mouse::getPosition(ref))))
	return true;
    }
  return false;
}

void	BarWidget::addSprite(t_sprite &elem)
{
  if (_sprites.size() == 1)
    {
      sf::FloatRect	barRect = getSprite(0).sprite.getGlobalBounds();
      sf::FloatRect	pickerRect = elem.sprite.getGlobalBounds();

      elem.sprite.setPosition(_zone.left,
			      barRect.top + barRect.height / 2.0 - pickerRect.height / 2.0);
    }
  else
    elem.sprite.setPosition(_zone.left, _zone.top);
  _sprites.push_back(elem);
}

void	BarWidget::addSprite(const sf::Texture &texture, const sf::IntRect &rect, bool display)
{
  t_sprite	sprite(sf::Sprite(texture, rect), display);

  addSprite(sprite);
}
