#include "TileMap.hpp"

TileMap::TileMap(const unsigned width, const unsigned height) :
  _tiles(width * height, TileType::Empty),
  _bgTiles(width * height, TileType::Empty),
  _width(width),
  _height(height)
{

}
