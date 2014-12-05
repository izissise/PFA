#ifndef	CHUNK_H
# define CHUNK_H

# include <array>
# include <cstdint>
# include <utility>
# include <vector>

# include <SFML/Graphics.hpp>
# include <SFML/System.hpp>

# include "TileCodex.hpp"
# include "TileType.hpp"
# include "Vector2.hpp"
# include "ProtocolMessage.pb.h"

using namespace google::protobuf;

class Chunk
{
public:
  static const int	width = 256;
  static const int	height = 256;

  Chunk(void);
  virtual ~Chunk(void);

  Chunk(const Chunk& other) = delete;
  Chunk&	operator=(const Chunk& other) = delete;

  void load(const TileCodex& codex);
  void fillTiles(const RepeatedField<uint32> &bgTiles,
		 const RepeatedField<uint32> &fgTiles);
  void draw(sf::RenderWindow& window,
	    Vector2i& windowCoord,
	    const TileCodex& codex) const;

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

private:
  void _generateVBO(const TileCodex& codex);

  std::vector<TileType>	_tiles;
  std::vector<TileType> _bgTiles;
  sf::VertexArray	_fgVertices;
  sf::VertexArray	_bgVertices;
  Vector2i		_pos;
  bool			_loaded;
};

#endif		/* CHUNK_H */
