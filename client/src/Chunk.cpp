#include <cmath>
#include <fstream>
#include <cstdio>
#include <random>
#include <functional>
// <TESTING ZONE>
#include <iostream>
#include <sstream>
// </TESTING ZONE>
#include "Unused.hpp"
#include "Chunk.hpp"
#include "SimplexNoise.h"

using namespace std;
using namespace noise;

Chunk::Chunk(void) :
  _tiles(width * height, TileType::Empty),
  _bgTiles(width * height, TileType::Empty),
  _loaded(false)
{
}

Chunk::~Chunk(void)
{
}

void Chunk::load(const TileCodex& codex)
{
  _generateVBO(codex);
  _loaded = true;
  _tiles.clear();
  _bgTiles.clear();
}

void	Chunk::fillTiles(const RepeatedField<uint32> &bgTiles,
			 const RepeatedField<uint32> &fgTiles)
{
  unsigned int x;
  unsigned int y;
  unsigned int index;

  for (y = 0; y < Chunk::height; ++y)
    for (x = 0; x < Chunk::width; ++x)
      {
	index = y * Chunk::width + x;
	_bgTiles[index] = static_cast<TileType>(bgTiles.Get(index));
	_tiles[index] = static_cast<TileType>(fgTiles.Get(index));
      }
}

void Chunk::_generateVBO(const TileCodex& codex)
{
  unsigned i;
  float x;
  std::pair<std::vector<TileType>&, sf::VertexArray&> layers[] =
    {
      {_bgTiles, _bgVertices},
      {_tiles,   _fgVertices}
    };
  for (auto layer : layers)
    {
      i = 0;
      layer.second.setPrimitiveType(sf::Quads);
      layer.second.resize(Chunk::width * Chunk::height * 4);
      for (float y = Chunk::height - 1; y >= 0; --y)
	{
	  for (x = 0.f; x < Chunk::width; ++x)
	    {
	      if (layer.first[i] != TileType::Empty)
		{
		  sf::Vertex *quad = &layer.second[i * 4];
		  quad[0].position = {x * TileCodex::tileSize, y * TileCodex::tileSize};
		  quad[1].position = {(x + 1.f) * TileCodex::tileSize, y * TileCodex::tileSize};
		  quad[2].position = {(x + 1.f) * TileCodex::tileSize, (y + 1.f) * TileCodex::tileSize};
		  quad[3].position = {x * TileCodex::tileSize, (y + 1.f) * TileCodex::tileSize};
		  codex.applySpriteUV(static_cast<unsigned>(layer.first[i]), quad);
		}
	      ++i;
	    }
	}
    }
}

void	Chunk::draw(sf::RenderWindow& window,
		    Vector2i& windowCoord,
		    const TileCodex& codex) const
{
  sf::RenderStates states(&codex.getTexture());

  states.transform.translate(windowCoord.x, windowCoord.y);
  states.shader = &codex.getBgShader();
  window.draw(_bgVertices, states);
  states.shader = nullptr;
  window.draw(_fgVertices, states);
}

void		Chunk::setPosition(const Vector2i &vec)
{
  _pos = vec;
}
