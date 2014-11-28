#ifndef	TILECODEX_H
# define TILECODEX_H

# include <SFML/Graphics.hpp>
# include <SFML/System.hpp>

# include <Vector2.hpp>

class TileCodex
{
public:
  static const unsigned tileSize = 16u;

  TileCodex(const std::string& path = "../client/assets/default.png");

  virtual ~TileCodex() = default;
  TileCodex(const TileCodex&) = delete;

  TileCodex&	operator=(const TileCodex&) = delete;

  void applySpriteUV(const unsigned id, sf::Vertex *quad) const;

  const sf::Texture& getTexture(void) const		{ return _spriteSheet; }
  const sf::Shader& getBgShader(void) const		{ return _bgShader; }
  unsigned getWidth(void) const				{ return _texSize.x; }
  unsigned getHeight(void) const			{ return _texSize.y; }

protected:
private:
  void		_initBgShader(void);

  std::vector<sf::Vector2f>	_spriteUVs;
  sf::Texture			_spriteSheet;
  sf::Shader			_bgShader;
  sf::Vector2i			_texSize;
};

#endif /* TILECODEX_H */
