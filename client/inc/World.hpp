#ifndef	WORLD_H
# define WORLD_H

# include <map>
# include <memory>

# include <SFML/Graphics.hpp>
# include <SFML/System.hpp>

# include <Box2D/Box2D.h>

# include "Settings.hpp"
# include "Camera.hpp"
# include "Player.hpp"

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
  ~World(void) = default;
  World(const World& other) = delete;
  World&		operator=(const World& other) = delete;

  void			update(void);
  void			draw(sf::RenderWindow& window) const;
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
				      const RepeatedField<uint32> &bgTiles,
				      const RepeatedField<uint32> &fgTiles);
  bool			getNewChunks(std::vector<Vector2i> &chunks);
  void			removeOldChunks();

protected:
private:
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
  Player	_player;
  std::unique_ptr<b2World>	_b2World;
  bool		_loaded;
};

#endif /* WORLD_H */
