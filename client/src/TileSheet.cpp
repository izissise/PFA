#include <stdexcept>

#include "TileSheet.hpp"

TileSheet::TileSheet(const std::string& path,
		     const unsigned spriteSize) :
  _spriteSize(spriteSize)
{
  // load sprite sheet
  if (not _spriteSheet.loadFromFile(path))
    throw std::invalid_argument("Invalid sprite sheet path");

  // generate sprites
  sf::IntRect	rect(0, 0, _spriteSize, _spriteSize);
  sf::Vector2u	texSize(_spriteSheet.getSize());

  for (rect.top = 0; rect.top < texSize.y; rect.top += _spriteSize)
    {
      for (rect.left = 0; rect.left < texSize.x; rect.left += _spriteSize)
	{
	  _sprites.push_back(sf::Sprite(_spriteSheet, rect));
	}
    }
}

void TileSheet::dumpSheet(sf::RenderWindow& window)
{
  float x = 0.0f;
  float size = _spriteSize;

  sf::Sprite bg(_spriteSheet, sf::IntRect(16, 16, 16, 16));
  window.draw(bg);
  for (auto& sprite : _sprites)
    {
      sprite.setPosition(x, 0.0f);
      window.draw(sprite);
      x += size;
    }
}
