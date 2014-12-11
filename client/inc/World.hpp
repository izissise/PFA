#ifndef	WORLD_H
# define WORLD_H

# include <map>
# include <memory>

# include <SFML/Graphics.hpp>
# include <SFML/System.hpp>

# include "Chunk.hpp"
# include "Settings.hpp"
# include "TileCodex.hpp"
# include "Camera.hpp"
# include "Player.hpp"

class WorldTester;

class World
{
  friend class WorldTester;

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

  void			movePlayer(const Vector2f &dir);
  void			setPlayerPosition(const Vector2f &position);
  const Player		&getPlayer() const; // use world's setter to set the client's attr
  void			fillChunkData(const VectorInt &pos,
				      const RepeatedField<uint32> &bgTiles,
				      const RepeatedField<uint32> &fgTiles);

protected:
private:
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
  Player	_player;
  bool		_loaded;
};

#endif /* WORLD_H */
