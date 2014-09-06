#include "AWidget.hpp"

AWidget::AWidget(const std::string &id, const sf::Vector2f &pos, sf::Text *text) :
  _id(id), _pos(pos), _text(text)
{
  if (_text)
    _text->setPosition(pos);
}

AWidget::~AWidget()
{
  if (_text)
    delete _text;
  _text = NULL;
}

void	AWidget::draw(sf::RenderWindow &window) const
{
  for (auto &sprite : _sprites)
    window.draw(sprite);
  if (_text)
    window.draw(*_text);
}

void	AWidget::addSprite(sf::Sprite &sprite)
{
  sprite.setPosition(_pos);
  _sprites.push_back(sprite);
}

void	AWidget::addSprite(sf::Texture * const texture, const sf::IntRect &rect)
{
  sf::Sprite	sprite(*texture, rect);

  addSprite(sprite);
}

void	AWidget::alignText(const sf::Vector2f &pos, const sf::Vector2f &size,
			   float xPercent, float yPercent)
{
  sf::Vector2f	npos(pos.x + (xPercent / 100.0) * size.x,
		     pos.y + (yPercent / 100.0) * size.y);
  sf::FloatRect	textSize = _text->getLocalBounds();

  npos.x -= textSize.width / 2.0;
  npos.y -= (textSize.height);
  _text->setPosition(npos.x, npos.y);
}

void	AWidget::setTextPosition(int x, int y)
{
  _text->setPosition(x, y);
}

void	AWidget::setTextAttr(unsigned int style)
{
  _text->setStyle(style);
}

bool	AWidget::isOver() const
{
  return false;
}

bool	AWidget::isClicked(const sf::Event &event) const
{
  return false;
}
