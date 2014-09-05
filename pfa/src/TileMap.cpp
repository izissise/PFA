#include "TileMap.hpp"

TileMap::TileMap(const unsigned width, const unsigned height) :
  _tiles(width * height, TileType::Ground),
  _width(width),
  _height(height)
{

}
