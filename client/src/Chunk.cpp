#include <cmath>
#include <fstream>
#include <string>
// <TESTING ZONE>
#include <iostream>
#include <cstdlib>
// </TESTING ZONE>

#include <boost/archive/binary_iarchive.hpp>

#include "Chunk.hpp"

const unsigned Chunk::width;
const unsigned Chunk::height;

using namespace std;

Chunk::Chunk() :
  _tiles(width * height, TileType::Empty),
  _bgTiles(width * height, TileType::Empty),
  _loaded(false)
{

}

void Chunk::loadFromFile(int xId, int yId, const TileCodex& codex)
{
  // std::ifstream ifs(std::to_string(x) + "_" + std::to_string(y) + ".chunk");

  // if (not ifs) {
  //   return ; // replace with exception
  // }
  // boost::archive::binary_iarchive ia(ifs);

  // ia >> _tiles;
  // ia >> _bgTiles;
  (void)xId, (void)yId;
  unsigned limit{Chunk::height * Chunk::width};
  unsigned i{0};
  for (; i < limit / 2; ++i) {
    _tiles[i] = TileType::Empty;
  }
  for (; i < limit; ++i) {
    _tiles[i] = TileType::Ground;
  }
  for (auto& tile : _bgTiles) {
    tile = TileType::Ground;
  }
  _boundingBox.left = xId - 0.5f;
  _boundingBox.top = yId - 0.5f;
  _boundingBox.width = 1.0f;
  _boundingBox.height = 1.0f;

  float x;

  std::pair<std::vector<TileType>&, sf::VertexArray&>	layers[] =
    {
      {_bgTiles, _bgVertices},
      {_tiles, _fgVertices}
    };
  for (auto layer : layers) {
    i = 0;
    layer.second.setPrimitiveType(sf::Quads);
    layer.second.resize(Chunk::width * Chunk::height * 4);
    for (float y = 0.f; y < Chunk::height; ++y) {
      for (x = 0.f; x < Chunk::width; ++x) {
	if (layer.first[i] != TileType::Empty) {
	  float tu = static_cast<int>(layer.first[i]) % codex.getWidth();
	  float tv = static_cast<int>(layer.first[i]) / codex.getWidth();
	  sf::Vertex *quad = &layer.second[i * 4];

	  quad[0].position = {x * TileCodex::tileSize, y * TileCodex::tileSize};
	  quad[1].position = {(x + 1.f) * TileCodex::tileSize, y * TileCodex::tileSize};
	  quad[2].position = {(x + 1.f) * TileCodex::tileSize, (y + 1.f) * TileCodex::tileSize};
	  quad[3].position = {x * TileCodex::tileSize, (y + 1.f) * TileCodex::tileSize};

	  quad[0].texCoords = {tu * TileCodex::tileSize, tv * TileCodex::tileSize};
	  quad[1].texCoords = {(tu + 1.f) * TileCodex::tileSize, tv * TileCodex::tileSize};
	  quad[2].texCoords = {(tu + 1.f) * TileCodex::tileSize, (tv + 1.f) * TileCodex::tileSize};
	  quad[3].texCoords = {tu * TileCodex::tileSize, (tv + 1.f) * TileCodex::tileSize};
	}
	++i;
      }
    }
  }
  _loaded = true;
}

void Chunk::draw(sf::RenderWindow& window,
		 const sf::Rect<float>& winBox,
		 sf::Vector2<int>& windowCoord,
		 const TileCodex& codex) const
{
  (void)winBox;
  (void)windowCoord;
  // sf::Vector2<unsigned>	origin;
  // sf::Vector2<unsigned> dest;
  // sf::Vector2<unsigned> cursor;
  // pair<const vector<TileType> *, const sf::Shader *> layers[] = {
  //   make_pair(&_bgTiles, &codex.getBgShader()),
  //   make_pair(&_tiles, nullptr)
  // };
  // TileType		tile;

  // origin.x = max(static_cast<int>(ceil((winBox.left - _boundingBox.left)
  // 				       / _boundingBox.width * Chunk::width)), 0);
  // origin.y = max(static_cast<int>(ceil((winBox.top - _boundingBox.top)
  // 				       / _boundingBox.height * Chunk::height)), 0);
  // dest.x = min(static_cast<unsigned>(ceil((winBox.left + winBox.width - _boundingBox.left)
  // 					  / _boundingBox.width * Chunk::width)), Chunk::width - 1);
  // dest.y = min(static_cast<unsigned>(ceil((winBox.top + winBox.height - _boundingBox.top)
  // 					  / _boundingBox.height * Chunk::height)), Chunk::height - 1);
  // for (auto layer : layers) {
  //   sf::RenderStates	rStates(layer.second);

  //   rStates.transform.translate(windowCoord.x, windowCoord.y);
  //   for (cursor = origin; cursor.y <= dest.y; ++cursor.y) {
  //     for (cursor.x = origin.x; cursor.x <= dest.x; ++cursor.x) {
  // 	if ((tile = (*layer.first)[cursor.y * Chunk::width + cursor.x]) != TileType::Empty) {
  // 	  window.draw(codex.getSprite(static_cast<unsigned>(tile)), rStates);
  // 	}
  // 	rStates.transform.translate(16.0f, 0.0f);
  //     }
  //     rStates.transform.translate(-16.0f * cursor.x, 16.0f);
  //   }
  // }
  sf::RenderStates states(&codex.getTexture());

  states.shader = &codex.getBgShader();
  window.draw(_bgVertices, states);
  states.shader = nullptr;
  window.draw(_fgVertices, states);
}
