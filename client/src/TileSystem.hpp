#ifndef	TILESYSTEM_H
# define TILESYSTEM_H

# include <SFML/Window.hpp>

# include "TileMap.hpp"
# include "TileSheet.hpp"

class TileSystem
{
public:
  TileSystem(const unsigned width, const unsigned height);
  TileSystem(const unsigned width, const unsigned height,
	     const std::string& path);
  TileSystem(const unsigned width, const unsigned height,
	     const std::string& path, const unsigned spriteSize);

  virtual ~TileSystem(void) = default;
  TileSystem(const TileSystem& other) = delete;
  TileSystem&	operator=(const TileSystem& other) = delete;

  void		render(sf::RenderWindow& window);

protected:
private:
  TileMap	_map;
  TileSheet	_sheet;
};

#endif /* TILESYSTEM_H */
