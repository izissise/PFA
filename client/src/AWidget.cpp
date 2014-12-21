#include "AWidget.hpp"
#include "FastMath.h"

AWidget::AWidget(const std::string &id, const sf::FloatRect &zone,
		 const sf::Text &text, wFlag flg) :
  _hide(false), _id(id), _zone(zone), _text(text), _flag(flg)
{
  _text.setPosition(zone.left, zone.top);
}

AWidget::~AWidget()
{
}

void		AWidget::draw(sf::RenderTexture &window) const
{
  sf::Shape	*edge = _edge.get();

  for (auto &elem : _sprites)
    if (elem.draw)
      window.draw(elem.sprite);
  if (edge)
    window.draw(*edge);
  window.draw(_text);
}

std::string	AWidget::getContent() const
{
  return (std::string());
}

void	AWidget::addSprite(t_sprite &elem)
{
  elem.sprite.setPosition(_zone.left, _zone.top);
  _sprites.push_back(elem);
}

void		AWidget::addSprite(const sf::Texture &texture, const sf::IntRect &rect, bool display)
{
  t_sprite	sprite(sf::Sprite(texture, rect), display);

  addSprite(sprite);
}

void	AWidget::alignText(const sf::Vector2f &pos, const sf::Vector2f &size,
			   float xPercent, float yPercent)
{
  sf::Vector2f	npos(pos.x + (xPercent / 100.0) * size.x,
		     pos.y + (yPercent / 100.0) * size.y);
  sf::FloatRect	textSize = _text.getGlobalBounds();
  sf::Vector2f	textScale = _text.getScale();

  npos.x -= textSize.width / 2.0;
  // get the true size of the font, then remove the blank spaces
  npos.y -= _text.getCharacterSize() * 0.68 * textScale.y;
  _text.setPosition(npos.x, npos.y);
}

void	AWidget::alignTextLeft(const sf::Vector2f &pos, const sf::Vector2f &size,
			       float xPercent, float yPercent)
{
  sf::Vector2f	npos(pos.x + (xPercent / 100.0) * size.x,
		     pos.y + (yPercent / 100.0) * size.y);

  npos.y -= (_text.getCharacterSize() / 1.5);
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

void	AWidget::setTextContent(const std::string &text)
{
  _text.setString(text);
}

const sf::String	&AWidget::getTextContent() const
{
  return _text.getString();
}

void	AWidget::setColor(const sf::Color &color)
{
  _text.setColor(color);
}

void	AWidget::setHidden(bool state)
{
  _hide = state;
}

bool	AWidget::isHidden() const
{
  return _hide;
}

void		AWidget::setUpdate(const std::function
				   <int (AWidget &widget, const sf::Event &event,
					 sf::RenderWindow &ref)> &func)
{
  _update = func;
}

void		AWidget::setTrigger(const std::function
				    <void (const t_event &event)> &func)
{
  _event = func;
}

void		AWidget::setEdge(std::unique_ptr<sf::RectangleShape> edgeShape,
				 float thickness,
				 const sf::Color &outColor)
{
  _edge = std::move(edgeShape);
  _edge->setPosition(_zone.left, _zone.top);
  _edge->setFillColor(sf::Color(127,127,127,0));
  _edge->setOutlineColor(outColor);
  _edge->setOutlineThickness(thickness);
}

sf::Shape	*AWidget::getEdge()
{
  return _edge.get();
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

void	AWidget::setSpriteAttr(unsigned int spritePos, bool display)
{
  _sprites[spritePos].draw = display;
}

const t_sprite	&AWidget::getSpriteAttr(unsigned int spritePos) const
{
  return _sprites[spritePos];
}

void	AWidget::toggleSpriteAttr(unsigned int spritePos)
{
  _sprites[spritePos].draw = !_sprites[spritePos].draw;
}

void	AWidget::scale(const sf::Vector2f &size)
{
  float	ratioX = size.x / SIZEX;
  float	ratioY = size.y / SIZEY;
  sf::Vector2f	textPos(_text.getPosition());
  sf::Vector2f	spritePos;
  float		thickness;

  _zone.left *= ratioX;
  _zone.top *= ratioY;
  _zone.width *= ratioX;
  _zone.height *= ratioY;
  for (auto &elem : _sprites)
    {
      spritePos = elem.sprite.getPosition();
      spritePos.x *= ratioX;
      spritePos.y *= ratioY;
      elem.sprite.setPosition(spritePos);
      elem.sprite.scale(ratioX, ratioY);
    }
  _text.scale(ratioX, ratioY);
  textPos.x *= ratioX;
  textPos.y *= ratioY;
  _text.setPosition(textPos);
  if (_edge)
    {
      _edge->setSize({_zone.width, _zone.height});
      _edge->setPosition({_zone.left, _zone.top});
      thickness = std::round(_edge->getOutlineThickness()
			     * (pointDistf(ratioX, ratioY) / pointDistf(1, 1)));
      if (thickness <= 0)
	thickness = 0.5f;
      _edge->setOutlineThickness(thickness);
    }
}

void	AWidget::resize(float pX, float pY)
{
  _zone.width *= pX;
  _zone.height *= pY;
  for (auto &elem : _sprites)
    {
      elem.sprite.scale(pX, pY);
    }
  _text.scale(pX, pY);
}

void		AWidget::setSize(float pX, float pY)
{
  float		rX = pX / _zone.width;
  float		rY = pY / _zone.height;

  for (auto &sprite : _sprites)
    {
      sprite.sprite.setScale(rX, rY);
    }
}

void	AWidget::setSpriteSize(unsigned int spritePos, float pX, float pY)
{
  sf::Sprite	&sprite = getSprite(spritePos).sprite;
  sf::FloatRect rec = sprite.getGlobalBounds();
  float		rX = pX / rec.width;
  float		rY = pY / rec.height;

  sprite.setScale(rX, rY);
}

void	AWidget::move(float pX, float pY)
{
  sf::Vector2f textPos(_text.getPosition());
  sf::Vector2f spritePos;

  _zone.left += pX;
  _zone.top += pY;
  for (auto &elem : _sprites)
    {
      spritePos = elem.sprite.getPosition();
      spritePos.x += pX;
      spritePos.y += pY;
      elem.sprite.setPosition(spritePos);
    }
  textPos.x += pX;
  textPos.y += pY;
  _text.setPosition(textPos);
}

t_sprite	&AWidget::getSprite(unsigned int spritePos)
{
  if (_sprites.size() <= spritePos)
    throw (std::out_of_range("Sprite out of range"));
  return _sprites[spritePos];
}

const sf::FloatRect	&AWidget::getZone() const
{
  return _zone;
}

wFlag	AWidget::getFlag() const
{
  return _flag;
}

const sf::String	&AWidget::getValue() const
{
  return _text.getString();
}

const std::string	&AWidget::getId() const
{
  return _id;
}

void	AWidget::trigger(const t_event &event)
{
  if (event.e & wEvent::SetSprite)
    {
      setSpriteAttr(event.idx, event.value);
    }
  else if (event.e & wEvent::Hide)
    {
      if (event.e & wEvent::Toggle)
	_hide = !_hide;
      else
	_hide = true;
    }
}
