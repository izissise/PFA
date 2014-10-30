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

void		Chunk::_constructBranch(sf::Vector2f pos, sf::Vector2f dir,
					int size, int cuSize, int thickness)
{
  if (cuSize > 0)
    {
      for (unsigned char branchSize = 0; branchSize < thickness; ++branchSize)
	{
	  if (std::abs(dir.x) < std::abs(dir.y))
	    _tiles[static_cast<int>(pos.y) * Chunk::width
		   + static_cast<int>(pos.x) + branchSize - thickness / 2] = TileType::Wood;
	  else
	    _tiles[(static_cast<int>(pos.y) + branchSize - thickness / 2) * Chunk::width
		   + static_cast<int>(pos.x)] = TileType::Wood;
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
			   (first) ? 0 : -1000, 1000) / 1000.0;
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
  float		ty;
  int		branchSize;
  unsigned char	v;

  for (char side = -1; side <= 1; side += 2)
    {
      ty = static_cast<float>(size) / 3.0;
      while (ty < size)
      	{
	  v = std::ceil((ty - static_cast<float>(size) / 3.f)
			/ ((static_cast<float>(size) - static_cast<float>(size) / 3.f) / (size / 15)));

	  v = ((v <= 0) ? 0 : (v - 1));
	  branchSize = _scaleNumber(std::rand(), 0, RAND_MAX, 1,
				    (thickness - ((v >= thickness) ? 1 : v)));
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
      sf::Vector2f t1(x, y + size - 3);
      sf::Vector2f t2(0.5 * side, 1);
      _constructBranch(t1, t2, size / 4, size / 4, (thickness == 1) ? 1 : thickness - 1);
    }
}

void		Chunk::_generateTree(float x, float y)
{
  int		size = _scaleNumber(std::rand(), 0, RAND_MAX, 25, 80);
  int		down;
  unsigned char	thickness = size / 15;

  for (int ty = 0; ty < size; ++ty)
    {
      for (int tx = 0; tx < thickness; ++tx)
	{
	  _tiles[(y + ty) * Chunk::width + (x + tx - thickness / 2)] = TileType::Wood;
	  if (ty == 0)
	    {
	      for (down = 1; _tiles[(y - down) * Chunk::width
				    + (x + tx - thickness / 2)] == TileType::Empty; ++down)
		_tiles[(y - down) * Chunk::width + (x + tx - thickness / 2)] = TileType::Wood;
	      if (_tiles[(y - down) * Chunk::width + (x + tx - thickness / 2)] != TileType::Vine)
		_tiles[(y - down) * Chunk::width + (x + tx - thickness / 2)] = TileType::Vine;
	    }
	}
    }
  _constructBranches(x, y, size, thickness);
}

void	Chunk::_getBiomeTile(unsigned int id, t_tileType &tile)
{
  if (_info[id].biome == Biome::Prairie)
    {
      tile.surface = TileType::Grass;
      tile.ground = TileType::Vine;
    }
  else if (_info[id].biome == Biome::Dry)
    {
      tile.surface = TileType::DryBlock;
      tile.ground = TileType::DryBlock;
    }
  else if (_info[id].biome == Biome::Desert)
    {
      tile.surface = TileType::Sand;
      tile.ground = TileType::Sand;
    }
  else if (_info[id].biome == Biome::Forest || _info[id].biome == Biome::Swamp)
    {
      tile.surface = TileType::ForestGrass;
      tile.ground = TileType::Vine;
    }
  else if (_info[id].biome == Biome::Tundra)
    {
      tile.surface = TileType::TundraGrass;
      tile.ground = TileType::Vine;
    }
  else
    {
      tile.surface = TileType::Snow;
      tile.ground = TileType::Vine;
    }
}

void		Chunk::_fillVertex(sf::Vector2f &prev, sf::Vector2f &next, int x)
{
  int		points = pow(2, iterations);
  int		s = TileCodex::tileSize;
  int		pos = (x * s) / ((static_cast<float>(LINELENGHT)
				  * static_cast<float>(Chunk::width)
				  * static_cast<float>(s))
				 / static_cast<float>(points));

  // if (_pos.x >= 0 && _pos.y == 0)
  //   std::cout << _pos.x << " " << x * s << " pos: " << pos << " "
  // 	      << _line.getPoint(pos).position.y  << " " <<  _line.getPoint(pos + 1).position.y  << std::endl;
  prev = sf::Vector2f(_line.getPoint(pos).position.x / s,
		      _line.getPoint(pos).position.y / s);
  pos += 1;
  next = sf::Vector2f(_line.getPoint(pos).position.x / s,
		      _line.getPoint(pos).position.y / s);
}

void		Chunk::_completeField(void)
{
  sf::Vector2f	prev;
  sf::Vector2f	next;
  float		a;
  float		b;
  float		p;
  float		biome;
  int		x;
  float		lineY;
  t_tileType	tile;
  float		part;
  float		scaledPosX;
  int		id = 0;
  int		oldId = -1;

  sf::Vector2f	offset = {static_cast<float>(Chunk::width) * _pos.x,
			  static_cast<float>(Chunk::height) * _pos.y};

  part = static_cast<float>(Chunk::width * LINELENGHT) / static_cast<float>(LOD);
  scaledPosX = (_pos.x >= 0 ? _pos.x : _pos.x + (_roundUpToMult(-_pos.x, LINELENGHT))) % LINELENGHT;
  for (int y = 0; y < Chunk::height; ++y)
    {
      for (x = 0; x < Chunk::width; ++x)
	{
	  oldId = -1;
	  id = static_cast<float>(x) / part;
	  if (id > oldId)
	    {
	      _getBiomeTile(id, tile);
	      oldId = id;
	    }
	  _fillVertex(prev, next, x + scaledPosX * Chunk::width);
	  if (y < prev.y || y < next.y)
	    {
	      a = (next.y - prev.y) / (next.x - prev.x);
	      b = next.y - a * next.x;
	      lineY = a * (x + scaledPosX * Chunk::width) + b;
	      if (x == Chunk::width / 2.0 && y >= lineY && y - 1 < lineY)
		1;//_generateTree(x, y);
	      else if (y < lineY && _tiles[y * Chunk::width + x] == TileType::Empty)
		{
		  p = octave_noise_2d(Chunk::octaves, PERSISTANCE, SCALE,
		  		      x + offset.x, y + offset.y);
		  if (p >= 0 - ((FADEH - ((lineY * TileCodex::tileSize + FADEH / 4
		  			   - y * TileCodex::tileSize >= FADEH)
		  			  ? FADEH : (lineY * TileCodex::tileSize + FADEH / 4
		  				     - y * TileCodex::tileSize)))
		  		/ FADEH))
		    {
		      if (y + 1 >= lineY)
			_tiles[y * Chunk::width + x] = tile.surface;
		      else
			_tiles[y * Chunk::width + x] = tile.ground;
		    }
		}
	    }
	}
    }
}

void	Chunk::_determineBiome(unsigned int id)
{
  int		minH = 0;
  int		biomeId = 1;
  unsigned int	i;
  std::array<t_BiomeInfo, 9>	biomes =
    {{
      {Biome::Dry, 0, 0, 20},
      {Biome::Desert, 0, 20, 50},
      {Biome::Prairie, 0, 50, 80},
      {Biome::Swamp, 0, 80, 100},
      {Biome::Desert, 250, 0, 50},
      {Biome::Prairie, 250, 50, 100},
      {Biome::Tundra, 500, 0, 50},
      {Biome::Forest, 500, 50, 100},
      {Biome::Ice, 750, 0, 100}
    }};

  for (i = 0; i < biomes.size(); ++i)
    {
      if ((_info[id].avHeight > biomes[i].minHeight ||
	   biomes[i].minHeight == minH) &&
	  _info[id].avHumidity >= biomes[i].minHumidity &&
	  _info[id].avHumidity <= biomes[i].maxHumidity)
	biomeId = i;
    }
  _info[id].biome = biomes[biomeId].name;
}

void	Chunk::_fillChunkInfo()
{
  float	moistL;
  float moistR;
  float	paddingX = 1.f / static_cast<float>(LOD);

  for (unsigned int id = 0; id < LOD; ++id)
    {
      moistL = _scaleNumber(raw_noise_2d(static_cast<float>(_pos.x) / 50.f, 0),
			    -1, 1, 0, 100);
      moistR = _scaleNumber(raw_noise_2d((static_cast<float>(_pos.x) + paddingX) / 50.f, 0),
			    -1, 1, 0, 100);
      _info[id].avHumidity = (moistL + moistR) / 2.f;
      _determineBiome(id);
    }
}

void		Chunk::_fillHeightMap()
{
  unsigned int	size = _line.size();
  float		part = static_cast<float>(size) / static_cast<float>(LOD);
  unsigned int	oldId = 0;
  unsigned int	id = 0;
  int		tHeight = 0;
  sf::Vertex	vertex;

  for (unsigned int i = 0; i < size; ++i)
    {
      id = static_cast<float>(i) / part;
      if (id > oldId)
	{
	  _info[oldId].avHeight = static_cast<float>(tHeight) / part;
	  oldId = id;
	  tHeight = 0;
	}
      vertex = _line.getPoint(i);
      tHeight += (vertex.position.y - MIDDLEHEIGHT
		  + (Chunk::height * TileCodex::tileSize * _pos.y));
    }
  _info[oldId].avHeight = static_cast<float>(tHeight) / part;
}

void	Chunk::_constructLine(void)
{
  list<sf::Vertex>::iterator	beg;
  sf::Vertex	prev;
  sf::Vertex	next;
  float		xOffset = Chunk::width * _pos.x;
  int		cutPoints = 1;
  float		mHeight;
  float		leftHeight;
  float		rightHeight;
  int		x;
  int		y;
  int		j;

  int	leftPoint = _roundDownToMult(_pos.x, LINELENGHT);
  int	rightPoint =  _roundUpToMult(_pos.x + 1, LINELENGHT);
  float	chunkWidth = Chunk::width * TileCodex::tileSize;
  float chunkHeight = Chunk::height * TileCodex::tileSize;

  leftHeight = VARIATION * raw_noise_2d(leftPoint, 0);
  rightHeight = VARIATION * raw_noise_2d(rightPoint, 0);
  mHeight = VARIATION * scaled_raw_noise_2d((leftPoint + rightPoint) / 2.f, 0, 0, 1);

  // Scale the line between [0, LINELENGHT]

  // if (_pos.x < 0 && _pos.y == 0)
  //   std::cout << "First: pos.x " <<  _pos.x << " pos.y " << _pos.y
  // 	      << " HEIGHT: " <<  (MIDDLEHEIGHT + leftHeight) << " "
  // 	      << (rightHeight + MIDDLEHEIGHT)  << " mHeight: " << mHeight << " "
  // 	      << leftPoint << " " << rightPoint
  // 	      << std::endl << std::endl;

  _line.points.push_back(sf::Vertex(sf::Vector2f
				    (0,
				     MIDDLEHEIGHT + leftHeight)));
  _line.points.push_back(sf::Vertex(sf::Vector2f
				    (LINELENGHT * chunkWidth,
				     MIDDLEHEIGHT + rightHeight)));

  for (unsigned int i = 0; i < iterations; ++i)
    {
      for (j = 0; j < cutPoints; ++j)
	{
	  beg = _line.points.begin();
	  prev = _line.getPointFromList(j * 2);
	  next = _line.getPointFromList(j * 2 + 1);
	  x = next.position.x / 2 + prev.position.x / 2;
	  y = next.position.y / 2 + prev.position.y / 2;
	  advance(beg, j * 2 + 1);
	  _line.points.insert(beg, sf::Vertex
			      (sf::Vector2f(x, y + mHeight
					    * raw_noise_2d(x + leftPoint * Chunk::width, 0))));
	}
      mHeight *= ROUGHNESS;
      cutPoints *= 2;
    }
  _line.update({0, static_cast<float>(chunkHeight * -_pos.y)});
  if (_pos.x < 0 && _pos.y == 0)
    for (unsigned int i = 0; i < _line.size(); ++i)
      {
  	std::cout << "dump: " <<_line.getPoint(i).position.x << " " << _line.getPoint(i).position.y << std::endl;
      }
}

void Chunk::_generate(void)
{
  unsigned int	i{0};
  float		x;
  float		p;
  Vector2f	offset = {static_cast<float>(Chunk::width) * _pos.x,
			  static_cast<float>(Chunk::height) * _pos.y};

  std::fill(_tiles.begin(), _tiles.end(), TileType::Empty);
  if (_pos.y >= 0)
    {
      _constructLine();
      _fillHeightMap();
      _fillChunkInfo();
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
  //_line.draw(window);
}
