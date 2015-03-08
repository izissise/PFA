#ifndef	CHUNK_H
# define CHUNK_H

# include <array>
# include <memory>
# include <cstdint>
# include <utility>
# include <vector>

# include <SFML/Graphics.hpp>
# include <SFML/System.hpp>

# include "Box2DHelpers.hpp"
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

  tile(TileType t, uint8_t l = 1) :
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

  void createFixture(std::shared_ptr<b2World> const& b2World);
  void load(const TileCodex& codex);
  void fillTiles(const ChunkData &packet);
  void draw(sf::RenderTarget& window,
	    Vector2i& windowCoord,
	    const TileCodex& codex) const;

  void		setPosition(const Vector2i &vec);

  tile		&getTile(unsigned index)	{ return _tiles[index]; }
  tile		&getBgTile(unsigned index)	{ return _bgTiles[index]; }
  bool		isLoaded(void) const			{ return _loaded; }
  bool		isGenerated() const			{ return _generated; }

  /*
  ** Conveniance wrappers
  */
  tile	&getTile(unsigned x, unsigned y)	{ return getTile(y * width + x); }
  tile	&getBgTile(unsigned x, unsigned y)	{ return getBgTile(y * width + x); }
  void	setTile(const Vector2i &pos, const tile &t,
		const TileCodex& codex);

  const Vector2i	&getPosition() const;

private:
  void	_generateVBO(const TileCodex& codex);

private:
  std::unique_ptr<b2Body, std::function<void(b2Body*)>> _body;
  std::vector<tile>	_tiles;
  std::vector<tile>	_bgTiles;
  sf::VertexArray	_fgVertices;
  sf::VertexArray	_bgVertices;
  Vector2i		_pos;
  bool			_generated;
  bool			_loaded;
};

#endif		/* CHUNK_H */
