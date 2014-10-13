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

# define PERSISTANCE 1.0f
# define SCALE 0.05f


class Chunk
{
public:
  static const unsigned width = 128;
  static const unsigned height = 128;
  static const unsigned octaves = 2;
  static const unsigned iterations = 5;

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
  void _loadFromFile(void);
  void _generateVBO(const TileCodex& codex);
  void _fillVertex(sf::Vector2f &prev, sf::Vector2f &next, int x);
  void _constructLine(void);
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
};

#endif		/* CHUNK_H */
