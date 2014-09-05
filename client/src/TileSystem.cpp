#include "TileSystem.hpp"

TileSystem::TileSystem(const unsigned width, const unsigned height) :
  _map(width, height)
{

}

TileSystem::TileSystem(const unsigned width, const unsigned height,
		       const std::string& path) :
  _map(width, height), _sheet(path)
{

}

TileSystem::TileSystem(const unsigned width, const unsigned height,
		       const std::string& path, const unsigned spriteSize) :
  _map(width, height), _sheet(path, spriteSize)
{

}

void TileSystem::render(sf::RenderWindow& window)
{
  unsigned x;
  unsigned index;
  TileType tile;
  unsigned size = _sheet.getSize();

  index = 0;
  for (unsigned y = 0; y < _map.getHeight(); ++y)
    {
      for (x = 0; x < _map.getWidth(); ++x)
	{
	  if ((tile = _map.getTile(index)) != TileType::Empty)
	    {
	      sf::Sprite& tileSprite = _sheet.getSprite(static_cast<unsigned>(tile));

	      tileSprite.setPosition(x * size, y * size);
	      window.draw(tileSprite);
	    }
	  ++index;
	}
    }
}
