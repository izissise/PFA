#ifndef	WORLD_H
# define WORLD_H

# include <map>

# include <SFML/Graphics.hpp>
# include <SFML/System.hpp>

# include "Chunk.hpp"
# include "Settings.hpp"
# include "TileCodex.hpp"

class World
{
public:
  World(Settings& settings);
  ~World(void) = default;
  World(const World& other) = delete;
  World&		operator=(const World& other) = delete;

  void			update(void);
  void			draw(sf::RenderWindow& window) const;

protected:
private:
  typedef sf::Vector2f worldPos;
  typedef sf::Vector2i screenPos;
  typedef std::pair<int, int> chunkId;

  class Camera
  {
  public:
    Camera(void) = default;
    ~Camera(void) = default;
    Camera(const Camera& other) = default;
    Camera&	operator=(const Camera& other) = default;

    void	translate(const sf::Vector2<float>& v) noexcept;
    void	move(const sf::Vector2<float>& pos) noexcept;

    void	scale(const sf::Vector2<float>& s) noexcept;
    void	resize(const sf::Vector2<float>& s) noexcept;

    float	left(void) const noexcept	{ return _topLeft.x; }
    float	right(void) const noexcept	{ return _topLeft.x + _size.x; }
    float	top(void) const noexcept	{ return _topLeft.y; }
    float	bottom(void) const noexcept	{ return _topLeft.y + _size.y; }
    float	width(void) const noexcept	{ return _size.x; }
    float	height(void) const noexcept	{ return _size.y; }
    worldPos	center(void) const noexcept	{ return _center; }

  private:
    worldPos	_topLeft;
    worldPos	_size;
    worldPos	_center;
  };

  worldPos		_sToWPos(screenPos pos) const;
  screenPos		_wToSPos(worldPos pos) const;
  float			_getGridOffset(float w) const;

  void			_drawChunk(sf::RenderWindow& window,
				   const chunkId& chunkCursor,
				   sf::Vector2<int>& windowCoord) const;
public:
  Camera			camera;

private:
  std::map<chunkId, Chunk *>	_chunks;
  Settings&			_settings;
  screenPos			_screenSize;
  TileCodex			_codex;
};

#endif /* WORLD_H */
