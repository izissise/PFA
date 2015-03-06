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
# include "Config.h"

using namespace google::protobuf;

struct	tile
{
  TileType	type;
  uint8_t	life;

  tile(TileType t, uint8_t l = 10) :
    type(t), life(l)
  {
  }
};

class Chunk
{
public:
  static const int	width = CHUNKWIDTH;
  static const int	height = CHUNKHEIGHT;
  static const unsigned	pWidth = width * TileCodex::tileSize;
  static const unsigned pHeight = height * TileCodex::tileSize;

  Chunk();
  Chunk(const Vector2i &chunkPos);
  virtual ~Chunk() = default;

  Chunk(const Chunk& other) = delete;
  Chunk&	operator=(const Chunk& other) = delete;

  void load(const TileCodex& codex);
  void fillTiles(const ChunkData &packet);
  void draw(sf::RenderTarget& window,
	    Vector2i& windowCoord,
	    const TileCodex& codex) const;

  void		setPosition(const Vector2i &vec);
  tile		getTile(unsigned index) const	{ return _tiles[index]; }
  void		setTile(unsigned index, tile t)	{ _tiles[index] = t; }
  tile		getBgTile(unsigned index) const	{ return _bgTiles[index]; }
  void		setBgTile(unsigned index, tile t)	{ _bgTiles[index] = t; }
  bool		isLoaded(void) const			{ return _loaded; }
  bool		isGenerated() const			{ return _generated; }

  /*
  ** Conveniance wrappers
  */
  tile	getTile(unsigned x, unsigned y) const	{ return getTile(y * width + x); }
  void	setTile(unsigned x, unsigned y, tile t)	{ setTile(y * width + x, t); }
  tile getBgTile(unsigned x, unsigned y) const	{ return getBgTile(y * width + x); }
  void setBgTile(unsigned x, unsigned y, tile t) { setBgTile(y * width + x, t); }

  const Vector2i	&getPosition() const;

private:
  void	_generateVBO(const TileCodex& codex);

  std::vector<tile>	_tiles;
  std::vector<tile>	_bgTiles;
  sf::VertexArray	_fgVertices;
  sf::VertexArray	_bgVertices;
  Vector2i		_pos;
  bool			_generated;
  bool			_loaded;
};

#endif		/* CHUNK_H */
