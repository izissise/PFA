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

Chunk::Chunk() :
  _tiles(width *height, TileType::Empty),
  _bgTiles(width *height, TileType::Empty),
  _generated(false),
  _loaded(false)
{
}

Chunk::Chunk(const Vector2i &chunkPos) :
  _tiles(width * height, TileType::Empty),
  _bgTiles(width * height, TileType::Empty),
  _pos(chunkPos),
  _generated(false),
  _loaded(false)
{
}

void Chunk::load(const TileCodex& codex)
{
  _generateVBO(codex);
  _loaded = true;
  _tiles.clear();
  _bgTiles.clear();
}

void	Chunk::fillTiles(const ChunkData &packet)
{
  unsigned int	index = 0;
  unsigned int	chunkLength = Chunk::height * Chunk::width;
  const google::protobuf::RepeatedField<uint32> &fgTile = packet.fgtilecode();
  const google::protobuf::RepeatedField<uint32> &bgTile = packet.bgtilecode();
  const google::protobuf::RepeatedField<uint32> &fgNumber = packet.fgnumber();
  const google::protobuf::RepeatedField<uint32> &bgNumber = packet.bgnumber();
  unsigned int	fgCounter = 0;
  unsigned int	bgCounter = 0;
  uint32	fgTileCounter = 0;
  uint32	bgTileCounter = 0;

  std::cout << "Filling chunk at pos " << _pos.x << " " << _pos.y << std::endl;
  for (index = 0; index < chunkLength; ++index)
    {
      if (fgTileCounter >= fgNumber.Get(fgCounter))
	{
	  fgTileCounter = 0;
	  ++fgCounter;
	}
      if (bgTileCounter >= bgNumber.Get(bgCounter))
	{
	  bgTileCounter = 0;
	  ++bgCounter;
	}
      _bgTiles[index] = tile(static_cast<TileType>(bgTile.Get(bgCounter)));
      _tiles[index] = tile(static_cast<TileType>(fgTile.Get(fgCounter)));
      ++fgTileCounter;
      ++bgTileCounter;
    }
  _generated = true;
}

void		Chunk::_generateVBO(const TileCodex& codex)
{
  int		y;
  unsigned int	x;
  unsigned int	idx = 0;

  _fgVertices.setPrimitiveType(sf::Quads);
  _bgVertices.setPrimitiveType(sf::Quads);
  _fgVertices.resize(Chunk::width * Chunk::height * 4);
  _bgVertices.resize(Chunk::width * Chunk::height * 4);
  for (y = Chunk::height - 1; y >= 0; --y)
    {
      for (x = 0; x < Chunk::width; ++x)
	{
	  if (_bgTiles[idx].type != TileType::Empty)
	    {
	      unsigned int	vIdx = idx * 4;
	      _bgVertices[vIdx].position = {static_cast<float>(x * TileCodex::tileSize),
					    static_cast<float>(y * TileCodex::tileSize)};
	      _bgVertices[vIdx + 1].position = {static_cast<float>((x + 1) * TileCodex::tileSize),
						static_cast<float>(y * TileCodex::tileSize)};
	      _bgVertices[vIdx + 2].position = {static_cast<float>((x + 1) * TileCodex::tileSize),
						static_cast<float>((y + 1) * TileCodex::tileSize)};
	      _bgVertices[vIdx + 3].position = {static_cast<float>(x * TileCodex::tileSize),
						static_cast<float>((y + 1) * TileCodex::tileSize)};
	      codex.applySpriteUV(static_cast<unsigned int>(_bgTiles[idx].type), &_bgVertices[vIdx]);
	    }
	  if (_tiles[idx].type != TileType::Empty)
	    {
	      unsigned int	vIdx = idx * 4;
	      _fgVertices[vIdx].position = {static_cast<float>(x * TileCodex::tileSize),
					    static_cast<float>(y * TileCodex::tileSize)};
	      _fgVertices[vIdx + 1].position = {static_cast<float>((x + 1) * TileCodex::tileSize),
						static_cast<float>(y * TileCodex::tileSize)};
	      _fgVertices[vIdx + 2].position = {static_cast<float>((x + 1) * TileCodex::tileSize),
						static_cast<float>((y + 1) * TileCodex::tileSize)};
	      _fgVertices[vIdx + 3].position = {static_cast<float>(x * TileCodex::tileSize),
						static_cast<float>((y + 1) * TileCodex::tileSize)};
	      codex.applySpriteUV(static_cast<unsigned int>(_tiles[idx].type), &_fgVertices[vIdx]);
	    }
	  ++idx;
	}
    }
}

void	Chunk::draw(sf::RenderTarget& window,
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

const Vector2i	&Chunk::getPosition() const
{
  return _pos;
}
