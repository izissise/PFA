#ifndef	CHUNK_H
# define CHUNK_H

# include <cstdint>
# include <utility>
# include <vector>

# include <SFML/Graphics.hpp>
# include <SFML/System.hpp>

# include "Lines.hpp"
# include "TileCodex.hpp"
# include "TileType.hpp"
# include "Vector2.hpp"
# include "Biomes.hpp"

# define PERSISTANCE 1.0f
# define SCALE 0.06f
# define LINELENGHT 3
# define MIDDLEHEIGHT 5000
# define VARIATION 2000.f
# define ROUGHNESS 0.5f // [0 - 1]
# define FADEH 300.0f
# define LOD 3
# define LS 1000.f

typedef struct	s_ChunkInfo
{
  int		avHumidity;
  int		avHeight;
  Biome		biome;
}		t_ChunkInfo;

typedef struct	s_tileType
{
  TileType	surface;
  TileType	ground;
}		t_tileType;

class Chunk
{
public:
  static const int width = 256;
  static const int height = 256;
  static const int octaves = 2;
  static const int iterations = 8;

  Chunk(void);
  virtual ~Chunk(void);

  Chunk(const Chunk& other) = delete;
  Chunk&	operator=(const Chunk& other) = delete;

  void load(int x, int y, const TileCodex& codex);
  void draw(sf::RenderWindow& window,
	    Vector2i& windowCoord,
	    const TileCodex& codex) const;

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

protected:
private:
  void _generate(void);
  void _determineBiome(unsigned int id);
  void _fillChunkInfo();
  void _fillHeightMap();
  void _generateTree(float x, float y);
  void _constructBranches(float x, float y, int size, int thickness);
  void _constructBranch(sf::Vector2f pos, sf::Vector2f dir,
			int size, int cuSize, int thickness);
  float _scaleNumber(float nb, float flb, float fhb,
		     float lb, float hb) { return (((hb - lb) * (nb - flb)) / (fhb - flb) + lb); }
  int	_roundUpToMult(int nb, int mult) { return (nb + (mult - (nb % mult)) % mult); }
  int	_roundNearToMult(int nb, int mult) { return (mult * std::round(nb / mult)); }
  int	_roundDownToMult(int nb, int mult) { return (nb >= 0 ?
						     mult * (nb / mult) :
						     -_roundUpToMult(-nb, mult)); }

  void _loadFromFile(void);
  void _generateVBO(const TileCodex& codex);
  void _fillVertex(sf::Vector2f &prev, sf::Vector2f &next, int x);
  void _constructLine(void);
  void _getBiomeTile(unsigned int id, t_tileType &tile);
  void _completeField(void);

  std::vector<TileType>	_tiles;
  std::vector<TileType> _bgTiles;
  bool			_loaded;
  sf::Font		_font;
  sf::Text		_id;
  Vector2i		_pos;
  sf::VertexArray	_fgVertices;
  sf::VertexArray	_bgVertices;
  Lines			_line;
  std::array<t_ChunkInfo, LOD>	_info;
};

#endif		/* CHUNK_H */
