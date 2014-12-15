#ifndef	CHUNK_H
# define CHUNK_H

# include <array>
# include <cstdint>
# include <cmath>
# include <utility>
# include <vector>

# include "Lines.hpp"
# include "TileType.hpp"
# include "TileCodex.hpp"
# include "Vector2.hpp"
# include "Biomes.hpp"
# include "Config.h"

# define LACUNARITY 2.5f
# define GAIN 0.6
# define OFFSET 1.02
# define SCALE 0.025f
# define LINELENGHT 4
# define MIDDLEHEIGHT 5000
# define MAXVARIATION 3000.f
# define MINVARIATION 500.f
# define ROUGHNESS 0.5f // [0 - 1]
# define FADEH 600.0f
# define LOD 15
# define PSCALE 50.f
# define HSCALE 25.f
# define OREDIST 8
# define SEED 15

enum class	Ore
{
  Coal,
    Iron,
    Copper,
    Diamond,
    Count
    };

typedef struct	s_ChunkInfo
{
  int		avHumidity;
  int		avHeight;
  Biome		biome;
}		t_ChunkInfo;

typedef struct	s_OreInfo
{
  Ore		name;
  TileType	tile;
  float		minNvalue;
  unsigned int	minSize;
  unsigned int	maxSize;
  int		minHeight;
  unsigned int	proportion;
  unsigned int	percentage;
}		t_OreInfo;

typedef struct	s_tileType
{
  TileType	surface;
  TileType	ground;
}		t_tileType;

class Chunk
{
public:
  static const int	width = CHUNKWIDTH;
  static const int	height = CHUNKHEIGHT;
  static const int	octaves = 2;
  static const int	iterations = 8;
  static const int	biomeMixDist = (width * LINELENGHT) / LOD;

  Chunk(void);
  Chunk(int xId, int yId);
  virtual ~Chunk(void);

  Chunk(const Chunk& other) = delete;
  Chunk&	operator=(const Chunk& other) = delete;

  void load(int x, int y);
  const Lines	&getLine() const;
  const Vector2i &getPosition() const	{ return _pos; }
  void		setPosition(const Vector2i &vec);

  TileType getTile(unsigned index) const	{ return _tiles[index]; }
  void setTile(unsigned index, TileType val)	{ _tiles[index] = val; }
  TileType getBgTile(unsigned index) const	{ return _bgTiles[index]; }
  void setBgTile(unsigned index, TileType val)	{ _bgTiles[index] = val; }
  bool isLoaded(void) const			{ return _loaded; }

  /*
  ** Conveniance wrappers
  */
  TileType getTile(unsigned x, unsigned y) const	{ return getTile(y * width + x); }
  void setTile(unsigned x, unsigned y, TileType val)	{ setTile(y * width + x, val); }
  TileType getBgTile(unsigned x, unsigned y) const	{ return getBgTile(y * width + x); }
  void setBgTile(unsigned x, unsigned y, TileType val)	{ setBgTile(y * width + x, val); }

  void		constructLine();
  void		fillChunkInfo();
  const t_ChunkInfo	&getChunkInfo(unsigned int pos) const;

protected:
private:
  void _generate(void);
  void _determineBiome(unsigned int id);
  void _fillHeightMap();
  void _generateTree(float x, float y);
  void _constructBranches(float x, float y, int size, int thickness);
  void _constructBranch(Vector2f pos, Vector2f dir,
			int size, int cuSize, int thickness);

  float _scaleNumber(float nb, float flb, float fhb,
		     float lb, float hb) const { return (((hb - lb) * (nb - flb)) / (fhb - flb) + lb); }
  int	_roundUpToMult(int nb, int mult) const  { return (nb + (mult - (nb % mult)) % mult); }
  int	_roundNearToMult(int nb, int mult) const { return (mult * std::round(nb / mult)); }
  int	_roundDownToMult(int nb, int mult) const { return (nb >= 0 ?
						     mult * (nb / mult) :
						     -_roundUpToMult(-nb, mult)); }
  int	_upScaleChunkPos(int num) const {
    return ((num >= 0 ?
	     num :
	     num + (_roundUpToMult(-num, LINELENGHT))) % LINELENGHT); }
  float	_maxNoise(float n1, float n2) const { return ((n1 > n2) ? (n1) : (n2)); }
  void _loadFromFile(void);
  void _fillVertex(Vector2f &prev, Vector2f &next, int x);
  void _getBiomeTile(Biome biome, t_tileType &tile);
  void _choseBiome(Biome * const biome, t_tileType &tile,
		 int x, int y, int &dist);
  void _completeField(void);
  void _generateOres();
  void _findOrePosition(const t_OreInfo &ore, int it);
  int  _getCellSubdivisor(int nbCell) const;
  bool _getOreRoot(const t_OreInfo &ore, int &x, int &y);
  void _fullScanPoint(int x, int y, int &dirX, int &dirY, int distance);
  void _checkAdjacentPoint(int x, int y, int &dirX, int &dirY, int distance);
  void _checkCornerPoint(int x, int y, int &dirX, int &dirY, int distance);
  bool _checkPoint(int x, int y, float &maxN) const;
  void _drawOres(const t_OreInfo &ore, float posX, float posY);

  std::vector<TileType>	_tiles;
  std::vector<TileType> _bgTiles;
  std::vector<float>	_chunkMap;
  bool			_loaded;
  Vector2i		_pos;
  Lines			_line;
  std::array<t_ChunkInfo, LOD>	_info;
};

#endif		/* CHUNK_H */
