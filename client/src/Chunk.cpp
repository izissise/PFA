#include <cmath>
#include <fstream>
// <TESTING ZONE>
#include <iostream>
#include <sstream>
// </TESTING ZONE>

#include "Chunk.hpp"
#include "SimplexNoise.h"

const unsigned Chunk::width;
const unsigned Chunk::height;

using namespace std;

Chunk::Chunk(void) :
  _tiles(width * height, TileType::Empty),
  _bgTiles(width * height, TileType::Empty),
  _loaded(false)
{
}

Chunk::~Chunk(void)
{
}

void Chunk::load(int xId, int yId, const TileCodex& codex)
{
  _pos = {xId, yId};
  _generate();
  _generateVBO(codex);
  _loaded = true;
}

void Chunk::_generate(void)
{
  unsigned i{0};
  float x;
  Vector2f offset = {static_cast<float>(Chunk::width) * _pos.x,
		     static_cast<float>(Chunk::height) * _pos.y};
  float p;

  if (_pos.y == 0) {
    _constructLine();
    _completeField();
  } else if (_pos.y > 0) {
    for (float y = 0; y < Chunk::height; ++y) {
      for (x = 0; x < Chunk::width; ++x) {
	p = octave_noise_2d(octaves, PERSISTANCE, SCALE,
			    x + offset.x, y + offset.y);
	if (p >= 0) {
	  if (p < 0.1) {
	    _tiles[i] = TileType::Vine;
	  } else {
	    _tiles[i] = TileType::Ground;
	  }
	}
	++i;
      }
    }
    for (TileType& tile : _bgTiles) {
      tile = TileType::Empty;
    }
  }
}

void Chunk::_loadFromFile(void)
{
  return ;
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

	  codex.applySpriteUV(static_cast<unsigned>(layer.first[i]), quad);
        }
        ++i;
      }
    }
  }
  _font.loadFromFile("../client/assets/font.otf");
  std::stringstream ss;
  ss << "(" << _pos.x << "," << _pos.y << ")";
  _id.setString(ss.str());
  _id.setCharacterSize(30);
  _id.setFont(_font);
}

void Chunk::draw(sf::RenderWindow& window,
                 Vector2i& windowCoord,
                 const TileCodex& codex) const
{
  sf::RenderStates states(&codex.getTexture());

  states.transform.translate(windowCoord.x, windowCoord.y);
  states.shader = &codex.getBgShader();
  window.draw(_bgVertices, states);
  states.shader = nullptr;
  window.draw(_fgVertices, states);
  window.draw(_id, states);
}

void		Chunk::_fillVertex(sf::Vector2f &prev, sf::Vector2f &next, int x)
{
  int		points = pow(2, iterations);
  int		pos = x / (1600.f / points);

  prev = sf::Vector2f(_line.getPoint(pos).position.x, _line.getPoint(pos).position.y);
  pos += 1;
  next = sf::Vector2f(_line.getPoint(pos).position.x, _line.getPoint(pos).position.y);
}

void		Chunk::_completeField(void)
{
  sf::Vector2f	prev;
  sf::Vector2f	next;
  float		a;
  float		b;
  unsigned	x;

  for (int y = static_cast<int>(Chunk::height) - 1; y >= 0; --y) {
    for (x = 0; x < Chunk::width; ++x) {
      _fillVertex(prev, next, x);
      if (y > prev.y && y > next.y) {
	_tiles[y * Chunk::width + x] = TileType::Ground;
      } else {
	a = (next.y - prev.y) / (next.x - prev.x);
	b = next.y - a * next.x;
	if (y > a * x + b) {
	  _tiles[y * Chunk::width + x] = TileType::Ground;
	}
      }
    }
  }
}

void	Chunk::_constructLine(void)
{
  list<sf::Vertex>::iterator	beg;
  sf::Vertex prev;
  sf::Vertex next;
  int	cutPoints;
  int	height;
  int	x;
  int	y;
  int	j;
  int	xOffset = Chunk::width * _pos.x;

  cutPoints = 1;
  if (_line.points.empty()) {
    _line.points.push_back(sf::Vertex(sf::Vector2f(0.0f, Chunk::height
						   * raw_noise_2d(_pos.x, _pos.y))));
    _line.points.push_back(sf::Vertex(sf::Vector2f(Chunk::width * TileCodex::tileSize,
						   Chunk::height * TileCodex::tileSize
						   * raw_noise_2d(_pos.x + 1, _pos.y))));
  }
  for (unsigned i = 0; i < iterations; ++i) {
    for (j = 0; j < cutPoints; ++j) {
      beg = _line.points.begin();
      prev = _line.getPointFromList(j * 2);
      next = _line.getPointFromList(j * 2 + 1);
      x = next.position.x / 2 + prev.position.x / 2;
      y = next.position.y / 2 + prev.position.y / 2;
      height = (Chunk::height /
		(static_cast<float>(i) * 2.f + 1.f)) * raw_noise_2d(x + xOffset, y);
      advance(beg, j * 2 + 1);
      _line.points.insert(beg, sf::Vertex(sf::Vector2f(x, y + height)));
    }
    cutPoints *= 2;
  }
  _line.update();
}
