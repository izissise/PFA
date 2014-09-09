#include "AWidget.hpp"

AWidget::AWidget(const std::string &id, const sf::FloatRect &zone,
		 const sf::Text &text) :
  _hide(false), _id(id), _zone(zone), _spritePos(-1), _text(text)
{
  _text.setPosition(zone.left, zone.top);
}

AWidget::~AWidget()
{
}

void	AWidget::draw(sf::RenderWindow &window) const
{
  if (_hide)
    return ;
  if (_spritePos > -1)
    window.draw(_sprites[_spritePos]);
  window.draw(_text);
}

void	AWidget::addSprite(sf::Sprite &sprite)
{
  sprite.setPosition(_zone.left, _zone.top);
  _sprites.push_back(sprite);
  if (_spritePos == -1)
    _spritePos = 0;
}

void	AWidget::addSprite(const sf::Texture &texture, const sf::IntRect &rect)
{
  sf::Sprite	sprite(texture, rect);

  addSprite(sprite);
}

void	AWidget::alignText(const sf::Vector2f &pos, const sf::Vector2f &size,
			   float xPercent, float yPercent)
{
  sf::Vector2f	npos(pos.x + (xPercent / 100.0) * size.x,
		     pos.y + (yPercent / 100.0) * size.y);
  sf::FloatRect	textSize = _text.getLocalBounds();

  npos.x -= textSize.width / 2.0;
  npos.y -= (textSize.height);
  _text.setPosition(npos.x, npos.y);
}

void	AWidget::alignTextLeft(const sf::Vector2f &pos, const sf::Vector2f &size,
			       float xPercent, float yPercent)
{
  sf::Vector2f	npos(pos.x + (xPercent / 100.0) * size.x,
		     pos.y + (yPercent / 100.0) * size.y);
  sf::FloatRect	textSize = _text.getLocalBounds();

  npos.y -= (textSize.height);
  _text.setPosition(npos.x, npos.y);
}

void	AWidget::setTextPosition(int x, int y)
{
  _text.setPosition(x, y);
}

void	AWidget::setTextAttr(unsigned int style)
{
  _text.setStyle(style);
}

void	AWidget::setHidden(bool state)
{
  _hide = state;
}

void		AWidget::setFunction(const std::string &key, const std::function
				     <int (AWidget &widget, const sf::Event &event,
					   sf::RenderWindow &ref)> &func)
{
  _updates[key] = func;
}

bool		AWidget::isOver(const sf::RenderWindow &ref) const
{
  return _zone.contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(ref)));
}

/*
** We consider isOver function is previously called
*/

bool	AWidget::isClicked(const sf::Event &event, sf::Mouse::Button button) const
{
  if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == button)
	return true;
    }
  return false;
}

void	AWidget::setSprite(unsigned int spritePos)
{
  _spritePos = spritePos;
}

void	AWidget::resize(const sf::Vector2f &size)
{
  float	ratioX = size.x / SIZEX;
  float	ratioY = size.y / SIZEY;
  sf::Vector2f textPos(_text.getPosition());
  sf::Vector2f spritePos;

  std::cout << _zone.left << std::endl;
  _zone.left *= ratioX;
  _zone.top *= ratioY;
  _zone.width *= ratioX;
  _zone.height *= ratioY;
  for (auto &elem : _sprites)
    {
      spritePos = elem.getPosition();
      spritePos.x *= ratioX;
      spritePos.y *= ratioY;
      elem.setPosition(spritePos);
      elem.setScale(ratioX, ratioY);
    }
  _text.setScale(ratioX, ratioY);
  textPos.x *= ratioX;
  textPos.y *= ratioY;
  _text.setPosition(textPos);
}

void	AWidget::trigger(const t_event &event)
{
  if (event.e & wEvent::SetSprite)
    {
      setSprite(event.additional);
    }
  else if (event.e & wEvent::Hide)
    {
      if (event.e & wEvent::Toggle)
	_hide = !_hide;
      else
	_hide = true;
    }
}
