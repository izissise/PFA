#include "AWidget.hpp"

AWidget::AWidget(const std::string &id, const sf::Vector2u &pos, sf::Text *text) :
  _id(id), _pos(pos), _text(text)
{
  if (_text)
    _text->setPosition(static_cast<sf::Vector2f>(pos));
}

AWidget::~AWidget()
{
  if (_text)
    delete _text;
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
  sprite.setPosition(static_cast<sf::Vector2f>(_pos));
  _sprites.push_back(sprite);
}
