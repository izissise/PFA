#ifndef	TILESHEET_H
# define TILESHEET_H

# include <SFML/Graphics.hpp>
# include <SFML/System.hpp>
# include <SFML/Window.hpp>

class TileSheet
{
public:
  TileSheet(const std::string& path = "../client/assets/default.png",
	    const unsigned spriteSize = 16);

  virtual ~TileSheet() = default;
  TileSheet(const TileSheet&) = delete;
  TileSheet&	operator=(const TileSheet&) = delete;

  inline sf::Sprite& getSprite(const unsigned id)
  {
    return _sprites[id];
  }

  inline unsigned getSize(void) const
  {
    return _spriteSize;
  }

  inline const sf::Texture& getTexture(void) const
  {
    return _spriteSheet;
  }

  void		dumpSheet(sf::RenderWindow& window);

protected:
private:
  sf::Texture			_spriteSheet;
  std::vector<sf::Sprite>	_sprites;
  const unsigned		_spriteSize;
};

#endif /* TILESHEET_H */
