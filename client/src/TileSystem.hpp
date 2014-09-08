#ifndef	TILESYSTEM_H
# define TILESYSTEM_H

# include <SFML/Window.hpp>

# include "TileMap.hpp"
# include "TileSheet.hpp"

class TileSystem
{
public:
  TileSystem(unsigned width, unsigned height);
  TileSystem(unsigned width, unsigned height,
	     const std::string& path);
  TileSystem(unsigned width, unsigned height,
	     const std::string& path, unsigned spriteSize);

  virtual ~TileSystem(void) = default;
  TileSystem(const TileSystem& other) = delete;
  TileSystem&	operator=(const TileSystem& other) = delete;

  void		render(sf::RenderWindow& window);

protected:
private:
  void		_initBgShader(void);
  void		_renderLayer(sf::RenderWindow& window, TileMap::Getter getter, sf::Shader *shader);

  TileMap	_map;
  TileSheet	_sheet;
  sf::Shader	_bgShader;
};

#endif /* TILESYSTEM_H */
