#include <cmath>
#include <fstream>
// <TESTING ZONE>
#include <iostream>
#include <sstream>
// </TESTING ZONE>

#include <boost/archive/binary_iarchive.hpp>

#include "Chunk.hpp"

const unsigned Chunk::width;
const unsigned Chunk::height;

using namespace std;

Chunk::Chunk() :
  _tiles(width * height, TileType::Empty),
  _bgTiles(width * height, TileType::Empty),
  _loaded(false) {

}

void Chunk::loadFromFile(int xId, int yId, const TileCodex& codex) {
  (void) xId, (void) yId;
  unsigned      i{0};
  for (unsigned b = 0; b < Chunk::height; ++b) {
    for (unsigned a = 0; a < Chunk::width; ++a) {
      if (a == 0 || b == 0) {
        _tiles[b * Chunk::width + a] = TileType::Ground;
      }
    }
  }
  for (TileType& tile : _bgTiles) {
    tile = TileType::Ground;
  }

  float x;
  std::pair<std::vector<TileType>&, sf::VertexArray&> layers[] =
    {
      {_bgTiles, _bgVertices},
      {_tiles,   _fgVertices}
    };

  for (auto layer : layers) {
    i = 0;
    layer.second.setPrimitiveType(sf::Quads);
    layer.second.resize(Chunk::width * Chunk::height * 4);
    for (float y = 0.f; y < Chunk::height; ++y) {
      for (x = 0.f; x < Chunk::width; ++x) {
        if (layer.first[i] != TileType::Empty) {
          sf::Vertex *quad = &layer.second[i * 4];

          quad[0].position = {x * TileCodex::tileSize, y * TileCodex::tileSize};
          quad[1].position = {(x + 1.f) * TileCodex::tileSize, y * TileCodex::tileSize};
          quad[2].position = {(x + 1.f) * TileCodex::tileSize, (y + 1.f) * TileCodex::tileSize};
          quad[3].position = {x * TileCodex::tileSize, (y + 1.f) * TileCodex::tileSize};

	  codex.applySpriteUV(static_cast<int>(layer.first[i]), quad);
        }
        ++i;
      }
    }
  }
  _font.loadFromFile("../client/assets/font.otf");
  std::stringstream ss;
  ss << "(" << xId << "," << yId << ")";
  _id.setString(ss.str());
  _id.setCharacterSize(30);
  _id.setFont(_font);
  _loaded = true;
}

void Chunk::draw(sf::RenderWindow& window,
                 Vector2i& windowCoord,
                 const TileCodex& codex) const {
  sf::RenderStates states(&codex.getTexture());

  states.transform.translate(windowCoord.x, windowCoord.y);
  states.shader = &codex.getBgShader();
  window.draw(_bgVertices, states);
  states.shader = nullptr;
  window.draw(_fgVertices, states);
  window.draw(_id, states);
}
