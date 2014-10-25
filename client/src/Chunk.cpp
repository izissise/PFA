#include <cmath>
#include <fstream>
#include <cstdio>
#include <random>
// <TESTING ZONE>
#include <iostream>
#include <sstream>
// </TESTING ZONE>

#include "Chunk.hpp"
#include "SimplexNoise.h"

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
  unsigned int	i{0};
  float		x;
  float		p;
  Vector2f	offset = {static_cast<float>(Chunk::width) * _pos.x,
			  static_cast<float>(Chunk::height) * _pos.y};

  if (_pos.y >= 0)
    {
      _constructLine();
      _completeField();
    }
  else if (_pos.y < 0)
    {
      std::fill(_bgTiles.begin(), _bgTiles.end(), TileType::Ground);
      for (float y = 0; y < Chunk::height; ++y)
	{
	  for (x = 0; x < Chunk::width; ++x)
	    {
	      p = octave_noise_2d(octaves, PERSISTANCE, SCALE,
				  x + offset.x, y + offset.y);
	      if (p >= 0)
		{
		  if (p < 0.1)
		    _tiles[i] = TileType::Vine;
		  else
		    _tiles[i] = TileType::Ground;
		}
	      ++i;
	    }
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
  _font.loadFromFile("../client/assets/font.otf");
  std::stringstream ss;
  ss << "(" << _pos.x << "," << _pos.y << ")";
  _id.setString(ss.str());
  _id.setCharacterSize(30);
  _id.setFont(_font);
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
  window.draw(_id, states);
}

void		Chunk::_fillVertex(sf::Vector2f &prev, sf::Vector2f &next, int x)
{
  int		points = pow(2, iterations);
  int		s = TileCodex::tileSize;
  int		pos = (x * s) / ((static_cast<float>(Chunk::width) *
				  static_cast<float>(s)) /
				 static_cast<float>(points));

  prev = sf::Vector2f(_line.getPoint(pos).position.x / s,
		      _line.getPoint(pos).position.y / s);
  pos += 1;
  next = sf::Vector2f(_line.getPoint(pos).position.x / s,
		      _line.getPoint(pos).position.y / s);
}

void		Chunk::_constructBranch(sf::Vector2f pos, sf::Vector2f dir,
					int size, int cuSize, int thickness)
{
  if (cuSize > 0)
    {
      char	side = 1;
      for (unsigned char branchSize = 0; branchSize < thickness; ++branchSize)
	{
	  if (std::abs(dir.x) < std::abs(dir.y))
	    _tiles[static_cast<int>(pos.y) * Chunk::width
		   + static_cast<int>(pos.x) + branchSize - thickness / 2] = TileType::Vine;
	  else
	    _tiles[(static_cast<int>(pos.y) + branchSize - thickness / 2) * Chunk::width
		   + static_cast<int>(pos.x)] = TileType::Vine;
	  side = -side;
	}
      pos.x += dir.x;
      pos.y += dir.y;
      _constructBranch(pos, dir, size, cuSize - 1, thickness);
    }
  else if ((thickness > 1 || cuSize == -1) && size >= 2)
    {
      bool	first = cuSize == -1;

      cuSize = size;
      size /= 1.5;
      dir.y = _scaleNumber(std::rand(), 0, RAND_MAX,
			   (first) ? -200: -1000, 1000) / 1000.0;
      _constructBranch(pos, dir, size, cuSize, thickness - 1 * !first);
      if (!first)
	{
	  dir.y = ((dir.y >= 0) ? 1 - dir.y : 1 + dir.y);
	  _constructBranch(pos, dir, size, cuSize, thickness / 2);
	}
    }
}

void		Chunk::_constructBranches(float x, float y, int size, int thickness)
{
  int		ty;
  int		branchSize;

  for (char side = -1; side <= 1; side += 2)
    {
      ty = static_cast<float>(size) / 3.0;
      while (ty < size)
      	{
	  branchSize = std::round(_scaleNumber(std::rand(), 0, RAND_MAX,
					       1,
					       (thickness - ty / (size / 2.0) < 1) ?
					       1 : thickness - ty / (size / 2.0)));
      	  if (_scaleNumber(std::rand(), 0, RAND_MAX, 0, 100) < 30)
      	    {
      	      sf::Vector2f t1(x + side * (thickness / 2 + 1)
			      - (side > 0 && thickness % 2 == 0), y + ty);
      	      sf::Vector2f t2(side, 0);
      	      _constructBranch(t1, t2, size / 4, -1, branchSize);
	      ty += 1 + branchSize * 2;
	    }
	  else
	    ty += 2;

      	}
      sf::Vector2f t1(x + 1, y + size - 1);
      sf::Vector2f t2(0.5 * side, 1);
      if (_scaleNumber(std::rand(), 0, RAND_MAX, 0, 10) < 8)
      _constructBranch(t1, t2, size / 4, -1, (thickness == 1) ? 1 : thickness - 1);
    }
}

void		Chunk::_generateTree(float x, float y)
{
  int		size = _scaleNumber(std::rand(), 0, RAND_MAX, 15, 100);
  unsigned char	thickness = size / 15;

  for (int ty = 0; ty < size; ++ty)
    {
      for (int tx = 0; tx < thickness; ++tx)
	_tiles[(y + ty) * Chunk::width + (x + tx - thickness / 2)] = TileType::Vine;
    }
  _constructBranches(x, y, size, thickness);
}

void		Chunk::_completeField(void)
{
  sf::Vector2f	prev;
  sf::Vector2f	next;
  float		a;
  float		b;
  float		p;
  unsigned int	x;
  int		lineY;

  sf::Vector2f	offset = {static_cast<float>(Chunk::width) * _pos.x,
			  static_cast<float>(Chunk::height) * _pos.y};

  for (int y = 0; y < Chunk::height; ++y)
    {
      for (x = 0; x < Chunk::width; ++x)
	{
	  _fillVertex(prev, next, x);
	  if (y < prev.y || y < next.y)
	    {
	      a = (next.y - prev.y) / (next.x - prev.x);
	      b = next.y - a * next.x;
	      lineY = a * x + b;
	      if (x == Chunk::width / 2.0 && y + 1 >= lineY && y < lineY)
		_generateTree(x, y);
	      else if (y < lineY)
		{
		  p = octave_noise_2d(Chunk::octaves, PERSISTANCE, SCALE,
				      x + offset.x, y + offset.y);
		  if (p >= 0 - ((FADEH - ((lineY * TileCodex::tileSize + FADEH / 4
					   - y * TileCodex::tileSize >= FADEH)
					  ? FADEH : (lineY * TileCodex::tileSize + FADEH / 4
						     - y * TileCodex::tileSize)))
				/ FADEH))
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
  int	xOffset = Chunk::width * _pos.x;
  int	chunkHeight = Chunk::height * TileCodex::tileSize;
  int	cutPoints;
  int	height;
  float	mheight;
  int	x;
  int	y;
  int	j;

  mheight = VARIATION * scaled_raw_noise_2d(0, 1.0, static_cast<float>(_pos.x) / 10.f, 0);
  cutPoints = 1;
  _line.points.push_back(sf::Vertex(sf::Vector2f
				    (0.f, MIDDLEHEIGHT + VARIATION
				     * raw_noise_2d(_pos.x * TileCodex::tileSize, 0))));
  _line.points.push_back(sf::Vertex(sf::Vector2f
				    (Chunk::width * TileCodex::tileSize,
				     MIDDLEHEIGHT + VARIATION
				     * raw_noise_2d((_pos.x + 1) * TileCodex::tileSize, 0))));
  for (unsigned int i = 0; i < iterations; ++i) {
    for (j = 0; j < cutPoints; ++j) {
      beg = _line.points.begin();
      prev = _line.getPointFromList(j * 2);
      next = _line.getPointFromList(j * 2 + 1);
      x = next.position.x / 2 + prev.position.x / 2;
      y = next.position.y / 2 + prev.position.y / 2;
      height = mheight * raw_noise_2d(x + xOffset, y);
      advance(beg, j * 2 + 1);
      _line.points.insert(beg, sf::Vertex(sf::Vector2f(x, y + height)));
    }
    mheight *= ROUGHNESS;
    cutPoints *= 2;
  }
  _line.update({0, static_cast<float>(chunkHeight * -_pos.y)});
}
