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
  typedef TileType (TileMap::*Getter)(unsigned) const;

  TileMap(unsigned width, unsigned height);
  ~TileMap() = default;

  TileMap(const TileMap&) = delete;
  TileMap&	operator=(const TileMap&) = delete;

  inline TileType getTile(unsigned index) const
  {
    return _tiles[index];
  }

  inline void setTile(unsigned index, TileType value)
  {
    _tiles[index] = value;
  }

  inline TileType getBgTile(unsigned index) const
  {
    return _bgTiles[index];
  }

  inline void setBgTile(unsigned index, TileType value)
  {
    _bgTiles[index] = value;
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
  inline TileType getTile(unsigned x, unsigned y) const
  {
    return getTile(y * _width + x);
  }

  inline void setTile(unsigned x, unsigned y, TileType value)
  {
    return setTile(y * _width + x, value);
  }

  inline TileType getBgTile(unsigned x, unsigned y) const
  {
    return getBgTile(y * _width + x);
  }

  inline void setBgTile(unsigned x, unsigned y, TileType value)
  {
    setBgTile(y * _width + x, value);
  }

protected:
private:
  std::vector<TileType>	_tiles;
  std::vector<TileType> _bgTiles;
  unsigned		_width;
  unsigned		_height;
};

#endif		/* TILEMAP_H */
