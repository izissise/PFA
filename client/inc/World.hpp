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

  void			translateCam(const sf::Vector2<float>& v);
  void			moveCam(const sf::Vector2<float>& pos);

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

    void	translate(const worldPos& v) noexcept;
    void	move(const worldPos& pos) noexcept;

    void	scale(const worldPos& s) noexcept;
    void	resize(const worldPos& s) noexcept;

    float	left(void) const noexcept	{ return _topLeft.x; }
    float	right(void) const noexcept	{ return _topLeft.x + _size.w; }
    float	top(void) const noexcept	{ return _topLeft.y; }
    float	bottom(void) const noexcept	{ return _topLeft.y + _size.h; }
    float	width(void) const noexcept	{ return _size.w; }
    float	height(void) const noexcept	{ return _size.h; }
    worldPos	center(void) const noexcept	{ return _center; }

  private:
    worldPos	_topLeft;
    worldPos	_size;
    worldPos	_center;
  };

  worldPos	_sToWPos(screenPos pos) const;
  screenPos	_wToSPos(worldPos pos) const;
  float		_getGridOffset(float w) const;

  void		_calculateVisibleRange(void);

  void		_drawChunk(sf::RenderWindow& window,
			   const chunkId& cursor,
			   screenPos& windowCoord) const;
  void		_loadChunks(void);
private:
  std::map<chunkId, std::unique_ptr<Chunk>>	_chunks;
  Settings&	_settings;
  TileCodex	_codex;
  Camera	_camera;
  Range2i	_loadedRange;
  Range2i	_visibleRange;
};

#endif /* WORLD_H */
