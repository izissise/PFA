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
  _chunkMap(width * height, 0),
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
  _loaded = true;
  _chunkMap.clear();
  _line.clear();
  _tiles.clear();
  _bgTiles.clear();
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

void	Chunk::_getBiomeTile(Biome biome, t_tileType &tile)
{
  if (biome == Biome::Prairie)
    {
      tile.surface = TileType::Grass;
      tile.ground = TileType::Vine;
    }
  else if (biome == Biome::Dry)
    {
      tile.surface = TileType::DryBlock;
      tile.ground = TileType::DryBlock;
    }

  else if (biome == Biome::Desert)
    {
      tile.surface = TileType::Sand;
      tile.ground = TileType::Sand;
    }
  else if (biome == Biome::Forest || biome == Biome::Swamp)
    {
      tile.surface = TileType::ForestGrass;
      tile.ground = TileType::Vine;
    }
  else if (biome == Biome::Tundra)
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

  prev = sf::Vector2f(_line.getPoint(pos).x / s,
		      _line.getPoint(pos).y / s);
  pos += 1;
  next = sf::Vector2f(_line.getPoint(pos).x / s,
		      _line.getPoint(pos).y / s);
}

void	Chunk::_choseBiome(Biome * const biome, t_tileType &tile,
			   int x, int y, int &dist)
{
  int	idx;

  if (y == 0)
    dist = (dist + 1) % Chunk::biomeMixDist;
  if (dist == 0)
    {
      _getBiomeTile(biome[1], tile);
      biome[0] = biome[1];
    }
  else
    {
      if (_scaleNumber((fbm_2d(1, 1, 0.5, SCALE, x - dist, y)),
      		       -1, 1, 0, static_cast<float>(Chunk::biomeMixDist) / 1.5f) > dist)
      	idx = 0;
      else
      	{
	  idx = _scaleNumber(std::rand(),
			     0, RAND_MAX, 0, Chunk::biomeMixDist);
	  idx = (idx < dist);
	}
      _getBiomeTile(biome[idx], tile);
    }
}

void		Chunk::_completeField(void)
{
  sf::Vector2f	prev;
  sf::Vector2f	next;
  float		a;
  float		b;
  float		p;
  float		off;

  float		lineY;
  t_tileType	tile;

  float		part;
  unsigned int	scaledPosX;
  Biome		biome[2];
  int		changeX = 0;
  float		y = 0;

  sf::Vector2f	offset = {static_cast<float>(Chunk::width) * _pos.x,
			  static_cast<float>(Chunk::height) * _pos.y};
  part = static_cast<float>(Chunk::width * LINELENGHT) / static_cast<float>(LOD);
  scaledPosX = _upScaleChunkPos(_pos.x);

  Chunk prevChunk;
  prevChunk.setPosition({_pos.x - 1, _pos.y});
  prevChunk.constructLine();
  prevChunk.fillChunkInfo();
  biome[0] = prevChunk.getChunkInfo((static_cast<float>(_upScaleChunkPos(scaledPosX - 1)
							* Chunk::width)
				     + (Chunk::width - 1)) / part).biome;

  for (int x = 0; x < Chunk::width; ++x)
    {
      y = 0;
      biome[1] = _info[static_cast<float>(x + scaledPosX * Chunk::width) / part].biome;
      _fillVertex(prev, next, x + scaledPosX * Chunk::width);
      a = (next.y - prev.y) / (next.x - prev.x);
      b = next.y - a * next.x;
      lineY = a * (x + scaledPosX * Chunk::width) + b;
      for (; y < Chunk::height; ++y)
	{
	  // second condition is to fill the tile if the border's biome are equal
	  if (biome[0] != biome[1] || (y == 0 && x == 0))
	    _choseBiome(biome, tile, x, y, changeX);
	  if (y < prev.y || y < next.y)
	    {
	      // if (x == Chunk::width / 2.0 && y >= lineY && y - 1 < lineY)
	      // 	_generateTree(x, y);
	      if (y < lineY && _tiles[y * Chunk::width + x] == TileType::Empty)
		{
		  off = OFFSET + 0.4 - ((fabs(lineY * TileCodex::tileSize
					      + offset.y * TileCodex::tileSize
					      - (y + offset.y) * TileCodex::tileSize) >= FADEH) ?
					(FADEH) :
					fabs(lineY * TileCodex::tileSize
					     + offset.y * TileCodex::tileSize
					     - (y + offset.y) * TileCodex::tileSize)) / FADEH / 2.5;
		  p = ridged_mf(Chunk::octaves, LACUNARITY, GAIN, SCALE, off,
				x + offset.x, y + offset.y);
		  _chunkMap[y * Chunk::width + x] = p;
		  if (p >= 0.5)
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
      {Biome::Desert, 400, 0, 50},
      {Biome::Prairie, 400, 50, 100},
      {Biome::Tundra, 800, 0, 50},
      {Biome::Forest, 800, 50, 100},
      {Biome::Ice, 1500, 0, 100}
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

void		Chunk::_fillHeightMap()
{
  unsigned int	size = _line.size();
  float		part = static_cast<float>(size) / static_cast<float>(LOD);
  unsigned int	oldId = 0;
  unsigned int	id = 0;
  int		tHeight = 0;
  Vector2f	vertex;

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
      tHeight += (vertex.y - MIDDLEHEIGHT
		  + (Chunk::height * TileCodex::tileSize * _pos.y));
    }
  _info[oldId].avHeight = static_cast<float>(tHeight) / part;
}

void	Chunk::fillChunkInfo()
{
  float	moistL;
  float moistR;
  float	part = 1.f / static_cast<float>(LOD);

  _fillHeightMap();
  for (unsigned int id = 0; id < LOD; ++id)
    {
      moistL = _scaleNumber(raw_noise_2d(((static_cast<float>(_pos.x)
					  + static_cast<float>(id) * part) / HSCALE), 0),
			    -1, 1, 0, 100);
      moistR = _scaleNumber(raw_noise_2d(((static_cast<float>(_pos.x)
					   + static_cast<float>(id + 1) * part) / HSCALE), 0),
			    -1, 1, 0, 100);
      _info[id].avHumidity = (moistL + moistR) / 2.f;
      _determineBiome(id);
    }
}

void	Chunk::constructLine()
{
  Vector2f	prev;
  Vector2f	next;
  unsigned int	size = std::pow(2, Chunk::iterations);
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

  _line.points.resize(size + 1);
  leftHeight = MAXVARIATION * raw_noise_2d
    (static_cast<float>(leftPoint) / PSCALE, 0);
  rightHeight = MAXVARIATION * raw_noise_2d
    (static_cast<float>(rightPoint) / PSCALE, 0);

  mHeight = MAXVARIATION * raw_noise_2d
    (((leftPoint + rightPoint) / 2.f) / PSCALE, 0);
  mHeight = _scaleNumber(mHeight, -MAXVARIATION, MAXVARIATION,
			 MINVARIATION, MAXVARIATION);

  _line.points[0] = Vector2f
    (0, MIDDLEHEIGHT + leftHeight
     - (chunkHeight * _pos.y));
  _line.points[size] = Vector2f
    (LINELENGHT * chunkWidth,
     MIDDLEHEIGHT + rightHeight
     - (chunkHeight * _pos.y));

  for (unsigned int i = 0; i < iterations; ++i)
    {
      for (j = 0; j < cutPoints; ++j)
	{
	  prev = _line.getPoint(j * size);
	  next = _line.getPoint((j + 1) * size);
	  x = next.x / 2 + prev.x / 2;
	  y = next.y / 2 + prev.y / 2;
	  _line.points[size / 2 + j * size] = Vector2f
	    (x, y + mHeight
	     * raw_noise_2d
	     ((x / Chunk::width + leftPoint), 0));
	}
      mHeight *= ROUGHNESS;
      cutPoints *= 2;
      size /= 2;
    }
  //  _line.update({0, static_cast<float>(chunkHeight * -_pos.y)});
}

int	Chunk::_getCellSubdivisor(int nbCell) const
{
  float	divisor[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // let subdivide max 100
  float	cuDiv;
  float	bDiv = 1;
  float	oldSpace = -1;

  for (unsigned int p = 0; p < sizeof(divisor) / sizeof(int); ++p)
    {
      cuDiv = static_cast<float>(nbCell) / divisor[p];
      if (cuDiv == 0)
	break ;
      if (cuDiv > static_cast<int>(cuDiv))
	continue ;
      if (oldSpace != -1 && abs(cuDiv - divisor[p]) > oldSpace)
	break ;
      bDiv = cuDiv;
      oldSpace = abs(cuDiv - divisor[p]);
    }
  return bDiv;
}

bool	Chunk::_checkPoint(int x, int y, float &maxN) const
{
  float	cuN;

  if (x >= 0 && y >= 0 && x < Chunk::width && y < Chunk::height)
    {
      if ((cuN = _maxNoise(maxN, _chunkMap[y * Chunk::width + x])) > maxN)
	{
	  maxN = cuN;
	  return true;
	}
    }
  return false;
}

void	Chunk::_fullScanPoint(int x, int y, int &dirX, int &dirY, int distance)
{
  int	tx;
  int	ty;
  float maxN = -1;

  tx = x + distance;
  ty = y + distance;
  if (_checkPoint(tx, ty, maxN) == true)
    {
      dirX = 1;
      dirY = 1;
    }
  tx -= 2 * distance;
  if (_checkPoint(tx, ty, maxN) == true)
    {
      dirX = -1;
      dirY = 1;
    }
  ty -= 2 * distance;
  if (_checkPoint(tx, ty, maxN) == true)
    {
      dirX = -1;
      dirY = -1;
    }
  tx += 2 * distance;
  if (_checkPoint(tx, ty, maxN) == true)
    {
      dirX = 1;
      dirY = -1;
    }
}

void	Chunk::_checkAdjacentPoint(int x, int y, int &dirX, int &dirY, int distance)
{
  int	tx;
  int	ty;
  float	maxN = -1;

  tx = x + dirX * distance;
  ty = y + dirY * distance;
  if (dirX)
    {
      for (ty = y - distance; ty <= y + distance; ty += distance)
	{
	  if (_checkPoint(tx, ty, maxN) == true)
	    dirY = ((ty - y < 0) ? -1 : (ty - y > 0) ? 1 : 0);
	}
    }
  else if (dirY)
    {
      for (tx = x - distance; tx <= x + distance; tx += distance)
	{
	  if (_checkPoint(tx, ty, maxN) == true)
	    dirX = ((tx - x < 0) ? -1 : (tx - x > 0) ? 1 : 0);
	}
    }
}

void	Chunk::_checkCornerPoint(int x, int y, int &dirX, int &dirY, int distance)
{
  int	tx;
  int	ty;
  float	maxN = -1;
  float	saveX = dirX;

  x = x + dirX * distance;
  y = y + dirY * distance;
  _checkPoint(x, y, maxN);
  tx = x - (dirX * distance);
  ty = y;
  if (_checkPoint(tx, ty, maxN))
    {
      dirX = 0;
    }
  tx = x;
  ty = y - (dirY * distance);
  if (_checkPoint(tx, ty, maxN))
    {
      dirX = saveX;
      dirY = 0;
    }
}

bool	Chunk::_getOreRoot(const t_OreInfo &ore, int &x, int &y)
{
  int	dirX = 0;
  int	dirY = 0;
  float	caseNoise;
  int	distance;

  for (int it = 0; it < OREDIST; ++it)
    {
      if (y >= 0 && y < Chunk::height && x >= 0 && x < Chunk::width)
	  caseNoise = _chunkMap[y * Chunk::width + x];
      else
	return false;
      if (caseNoise >= ore.minNvalue)
	return true;
      distance = 2.5 + ((ore.minNvalue - caseNoise) * 10.0) / 1.5;
     if (it == 0) // Full scan needed
       _fullScanPoint(x, y, dirX, dirY, distance);
      else // just test the 2 adjacents cases
	{
	  if (dirX == 0 || dirY == 0)
	    _checkAdjacentPoint(x, y, dirX, dirY, distance);
	  else
	    _checkCornerPoint(x, y, dirX, dirY, distance);
	}
     x += dirX * distance;
     y += dirY * distance;
    }
  return false;
}

void	Chunk::_drawOres(const t_OreInfo &ore, float x, float y)
{
  unsigned int	cellNb;
  float		side;
  float		stat;
  float		randFactor;

  cellNb = _scaleNumber(std::rand(), 0, RAND_MAX, ore.minSize, ore.maxSize);
  for (side = 1; side * side <= cellNb && (side + 1) * (side + 1) < cellNb; ++side);
  for (float ty = y - side / 2; ty <= y + side / 2; ++ty)
    {
      for (float tx = x - side / 2; tx <= x + side / 2; ++tx)
  	{
  	  if (tx >= 0 && ty >= 0 && tx < Chunk::width && ty < Chunk::height
  	      && _chunkMap[static_cast<int>(ty) * Chunk::width
			   + static_cast<int>(tx)] >= ore.minNvalue)
  	    {
  	      stat = _scaleNumber((fabs(tx - x) + fabs(ty - y)) / 2, 0, side / 2, 0, 50);
  	      randFactor = _scaleNumber(std::rand(), 0, RAND_MAX, 0, 0.6);
  	      if (50.f + (stat * 0.4) + (stat * randFactor) < 75)
		_tiles[static_cast<int>(ty) * Chunk::width + static_cast<int>(tx)] = ore.tile;
  	    }
  	}
    }
}

void	Chunk::_findOrePosition(const t_OreInfo &ore, int it)
{
  int	div;
  float	cellX;
  float	cellY;
  int	posX;
  int	posY;

  div = _getCellSubdivisor(ore.proportion);
  cellX = Chunk::width / div;
  cellY = Chunk::height / (ore.proportion / div);
  posX = (it % div) * cellX;
  posY = (it / div) * cellY;
  // This gets me a random position in the subcell n It
  posX += _scaleNumber(std::rand(), 0, RAND_MAX, 0, cellX);
  posY += _scaleNumber(std::rand(), 0, RAND_MAX, 0, cellY);
  if (_getOreRoot(ore, posX, posY))
    _drawOres(ore, posX, posY);
}

void	Chunk::_generateOres()
{
  const t_OreInfo	ores[static_cast<int>(Ore::Count)] =
    {
      {Ore::Coal,TileType::CoalOre, 0.5, 16, 24, 0, 500, 100},
      {Ore::Iron, TileType::IronOre, 0.5, 10, 16, -2, 300, 80},
      {Ore::Copper, TileType::GoldOre, 0.5, 10, 16, -3, 300, 80},
      {Ore::Diamond, TileType::DiamondOre, 0.9, 5, 9, -5, 200, 50},
    };
  unsigned int	randNb;

  for (int i = sizeof(ores) / sizeof(t_OreInfo) - 1; i >= 0; --i)
    {
      if (_pos.y < ores[i].minHeight)
	{
	  for (unsigned int n = 0; n < ores[i].proportion; ++n)
	    {
	      randNb = _scaleNumber(rand(), 0, RAND_MAX, 0, 100);
	      if (randNb <= ores[i].percentage)
		_findOrePosition(ores[i], n);
	    }
	}
    }
}

void	Chunk::_generate(void)
{
  unsigned int	i{0};
  float		x;
  float		p;
  Vector2f	offset = {static_cast<float>(Chunk::width) * _pos.x,
			  static_cast<float>(Chunk::height) * _pos.y};

  std::fill(_tiles.begin(), _tiles.end(), TileType::Empty);
  if (_pos.y >= 0)
    {
      constructLine();
      fillChunkInfo();
      _completeField();
    }
  else if (_pos.y < 0)
    {
      std::fill(_bgTiles.begin(), _bgTiles.end(), TileType::Ground);
      for (float y = 0; y < Chunk::height; ++y)
	{
	  for (x = 0; x < Chunk::width; ++x)
	    {
	      p = ridged_mf(Chunk::octaves, LACUNARITY, GAIN, SCALE, OFFSET,
			 (x + offset.x) / 1.0, (y + offset.y) / 1.0);
	      _chunkMap[y * Chunk::width + x] = p;
	      if (p >= 0.5)
		{
		  if (p < 0.1)
		    _tiles[i] = TileType::Vine;
		  else
		    _tiles[i] = TileType::Ground;
		}
	      ++i;
	    }
	}
      _generateOres();
    }
}

void Chunk::_loadFromFile(void)
{
  return ;
}

const Lines	&Chunk::getLine() const
{
  return _line;
}

void		Chunk::setPosition(const Vector2i &vec)
{
  _pos = vec;
}

const t_ChunkInfo	&Chunk::getChunkInfo(unsigned int pos) const
{
  if (pos >= LOD)
    throw (std::out_of_range(""));
  return _info[pos];
}