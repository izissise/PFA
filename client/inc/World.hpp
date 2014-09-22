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
  World(Settings& set);
  virtual ~World(void) = default;
  World(const World& other) = delete;
  World&	operator=(const World& other) = delete;

  void			draw(sf::RenderWindow& window) const;

  void			translateCamera(const sf::Vector2<float>& v);
  void			moveCamera(const sf::Vector2<float>& pos);

protected:
private:
  Chunk			*_getChunk(int x, int y);
  void			_drawChunk(sf::RenderWindow& window,
				   const std::pair<int, int>& chunkCursor,
				   sf::Vector2<int>& windowCoord) const;

  std::map<std::pair<int, int>, Chunk *>	_chunks;
  Settings&		_set;
  sf::Rect<float>	_box;
  sf::Vector2<float>	_center;
  TileCodex		_codex;
};

#endif /* WORLD_H */
