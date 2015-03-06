#ifndef	WORLD_H
# define WORLD_H

# include <map>
# include <memory>
# include <deque>
# include <chrono>

# include <SFML/Graphics.hpp>
# include <SFML/System.hpp>

# include <Box2D/Box2D.h>

# include "Box2DHelpers.hpp"
# include "Settings.hpp"
# include "Camera.hpp"
# include "Player.hpp"
# include "Chunk.hpp"

class WorldTester;

class World
{
  friend class WorldTester;

public:
  const unsigned int	cacheSize = 1;

private:
  typedef Vector2f	worldPos;
  typedef Vector2i	screenPos;
  typedef Vector2i	chunkId;

public:
  World(Settings& settings);
  ~World() = default;
  World(const World& other) = delete;
  World&		operator=(const World& other) = delete;

  void			update(const std::chrono::milliseconds& timeStep);
  void			draw(sf::RenderTarget& window) const;

  void			load();
  void			loadRange();

  bool			movePlayer(const Vector2f &dir);
  bool			movePlayer(const VectorInt &chunkId,
				   const VectorFloat &pos);
  void			setPlayerPosition(const Vector2i &chunkId,
					  const Vector2f &position);
  void			setPlayerPosition(const VectorInt &chunkId,
					  const VectorFloat &position)
  {
    setPlayerPosition(Vector2i(chunkId.x(), chunkId.y()),
		      Vector2f(position.x(), position.y()));
  }
  const Player		&getPlayer() const; // use world's setter to set the client's attr
  bool			isLoaded() const	{ return _loaded; }
  bool			isChunkLoaded(const Vector2i &chunkPos) const;
  void			fillChunkData(const VectorInt &pos,
				      const ChunkData &packet);
  bool			getNewChunks(std::vector<Vector2i> &chunks);

protected:
private:
  float		_getGridOffset(float w) const;
  void		_calculateVisibleRange(void);
  screenPos	_getScreenOrigin(void) const;
  void		_drawBackground(sf::RenderTarget &window) const;
  void		_drawChunk(sf::RenderTarget &window,
			   const chunkId& cursor,
			   screenPos& windowCoord) const;
  void		_loadChunks();
  void		_unloadChunks();
  void		_processHitAction(const Controls &ctrl);

private:
  std::shared_ptr<b2World>	_b2World;
  std::map<chunkId, std::unique_ptr<Chunk>>	_chunks;
  Settings&	_settings;
  screenPos	_screenSize;
  TileCodex	_codex;
  Camera	_camera;
  std::deque<std::shared_ptr<AEntity>> _entities;
  std::unique_ptr<b2Body, std::function<void(b2Body*)>> _tmpGround;
  Player	_player;
  bool		_loaded;
};

#endif /* WORLD_H */
