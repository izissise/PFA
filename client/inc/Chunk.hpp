#ifndef	CHUNK_H
# define CHUNK_H

# include <cstdint>
# include <utility>
# include <vector>

# include <SFML/Graphics.hpp>
# include <SFML/System.hpp>

# include "TileCodex.hpp"
# include "TileType.hpp"
# include "Vector2.hpp"

class Chunk
{
public:
  static const unsigned width = 128;
  static const unsigned height = 128;

  typedef TileType (Chunk::*Getter)(unsigned) const;

  Chunk(void);
  virtual ~Chunk() = default;

  Chunk(const Chunk& other) = delete;
  Chunk&	operator=(const Chunk& other) = delete;

  void loadFromFile(int x, int y, const TileCodex& codex);
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
  std::vector<TileType>	_tiles;
  std::vector<TileType> _bgTiles;
  bool			_loaded;
  sf::Font		_font;
  sf::Text		_id;
  sf::VertexArray	_fgVertices;
  sf::VertexArray	_bgVertices;
};

#endif		/* CHUNK_H */
