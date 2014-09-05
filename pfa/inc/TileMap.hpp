#ifndef	TILEMAP_H
# define TILEMAP_H

# include <cstdint>
# include <vector>

/*
** TileType is currently defined as an 8-bit identifier
** It should be noted that it must be changed if
** over 256 different tile types are needed.
*/
enum class TileType : std::int8_t
{
  Empty,
    Ground
    };

class TileMap
{
public:
  TileMap(const unsigned width, const unsigned height);
  ~TileMap() = default;

  TileMap(const TileMap&) = delete;
  TileMap&	operator=(const TileMap&) = delete;

  inline TileType getTile(const unsigned index) const
  {
    return _tiles[index];
  }

  inline void setTile(const unsigned index, const TileType value)
  {
    _tiles[index] = value;
  }

  inline unsigned getWidth(void) const
  {
    return _width;
  }

  inline unsigned getHeight(void) const
  {
    return _height;
  }

  /*
  ** Conveniance wrappers
  */
  inline TileType getTile(const unsigned x, const unsigned y) const
  {
    return getTile(y * _width + x);
  }

  inline void setTile(const unsigned x, const unsigned y, const TileType value)
  {
    return setTile(y * _width + x, value);
  }

protected:
private:
  std::vector<TileType>	_tiles;
  unsigned		_width;
  unsigned		_height;
};

#endif		/* TILEMAP_H */
