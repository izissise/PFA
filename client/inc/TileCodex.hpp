#ifndef	TILECODEX_H
# define TILECODEX_H

# include <SFML/Graphics.hpp>
# include <SFML/System.hpp>

class TileCodex
{
public:
  static const unsigned tileSize = 16u;

  TileCodex(const std::string& path = "../client/assets/default.png");

  virtual ~TileCodex() = default;
  TileCodex(const TileCodex&) = delete;
  TileCodex&	operator=(const TileCodex&) = delete;

  const sf::Sprite& getSprite(const unsigned id) const	{ return _sprites[id]; }
  const sf::Texture& getTexture(void) const		{ return _spriteSheet; }
  const sf::Shader& getBgShader(void) const		{ return _bgShader; }
  const unsigned& getWidth(void) const			{ return _texSize.x; }
  const unsigned& getHeight(void) const			{ return _texSize.y; }

protected:
private:
  void		_initBgShader(void);

  sf::Texture			_spriteSheet;
  std::vector<sf::Sprite>	_sprites;
  sf::Shader			_bgShader;
  sf::Vector2u			_texSize;
};

#endif /* TILECODEX_H */
