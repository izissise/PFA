#ifndef	WORLD_H
# define WORLD_H

# include <map>
# include <memory>

# include <SFML/Graphics.hpp>
# include <SFML/System.hpp>

# include "Chunk.hpp"
# include "Range2.hpp"
# include "Settings.hpp"
# include "TileCodex.hpp"
# include "Vector2.hpp"

class WorldTester;

class World
{
  friend class WorldTester;

public:
  World(Settings& settings);
  ~World(void) = default;
  World(const World& other) = delete;
  World&		operator=(const World& other) = delete;

  void			update(void);
  void			draw(sf::RenderWindow& window) const;
  void			load();

  void			movePlayer(const Vector2f &dir);
  void			setPlayerPosition(const Vector2f &position);
  const Vector2f	&getPlayerPosition() const;
  void			fillChunkData(const VectorInt &pos,
				      const RepeatedField<uint32> &bgTiles,
				      const RepeatedField<uint32> &fgTiles);

protected:
private:
  typedef Vector2f	worldPos;
  typedef Vector2i	screenPos;
  typedef Vector2i	chunkId;

  class Camera
  {
  public:
    Camera(void) = default;
    ~Camera(void) = default;
    Camera(const Camera& other) = default;
    Camera&	operator=(const Camera& other) = default;

    void	translate(const worldPos& v);
    void	move(const worldPos& pos);

    void	scale(const worldPos& s);
    void	resize(const worldPos& s);

    float	left(void) const	{ return _botLeft.x; }
    float	right(void) const	{ return _botLeft.x + _size.w; }
    float	top(void) const		{ return _botLeft.y + _size.h; }
    float	bottom(void) const	{ return _botLeft.y; }
    float	width(void) const	{ return _size.w; }
    float	height(void) const	{ return _size.h; }
    worldPos	center(void) const	{ return _center; }

  private:
    worldPos	_botLeft;
    worldPos	_size;
    worldPos	_center;
  };

  worldPos	_sToWPos(screenPos pos, bool noOffsets = false) const;
  screenPos	_wToSPos(worldPos pos, bool noOffsets = false) const;
  float		_getGridOffset(float w) const;

  void		_calculateVisibleRange(void);

  screenPos	_getScreenOrigin(void) const;
  void		_drawChunk(sf::RenderWindow& window,
			   const chunkId& cursor,
			   screenPos& windowCoord) const;
  void		_loadChunks(void);

private:
  std::map<chunkId, std::unique_ptr<Chunk>>	_chunks;
  Settings&	_settings;
  screenPos	_screenSize;
  TileCodex	_codex;
  Camera	_camera;
  Range2i	_loadedRange;
  Range2i	_visibleRange;
  bool		_loaded;
};

#endif /* WORLD_H */
