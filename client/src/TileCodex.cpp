#include <stdexcept>

// <TESTING ZONE>
#include <iostream>
// </TESTING ZONE>

#include "TileCodex.hpp"

const unsigned TileCodex::tileSize;

TileCodex::TileCodex(const std::string& path)
{
  // load sprite sheet
	if (!_spriteSheet.loadFromFile(path)) {
    throw std::invalid_argument("Invalid sprite sheet path");
  }

  // generate sprites
  Vector2f	tile(0.0f, 0.0f);
  _texSize.x = _spriteSheet.getSize().x;
  _texSize.y = _spriteSheet.getSize().y;

  for (; tile.y < _texSize.y; tile.y += tileSize) {
    for (tile.x = 0.0f; tile.x < _texSize.x; tile.x += tileSize) {
      _spriteUVs.emplace_back(tile.x, tile.y);
      _spriteUVs.emplace_back((tile.x + tileSize), tile.y);
      _spriteUVs.emplace_back((tile.x + tileSize), (tile.y + tileSize));
      _spriteUVs.emplace_back(tile.x, (tile.y + tileSize));
    }
  }

  // load background shader
  /*
  ** sorry for just putting it in the code like this,
  ** i'll move it as soon as we have a good file structure
  ** for loading assets at runtime.
  */
  _bgShader.loadFromMemory("uniform sampler2D texture;"
			  ""
			  "void main()"
			  "{"
			  "  vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);"
			  "  gl_FragColor = gl_Color * pixel * vec4(0.5, 0.5, 0.5, 1.0);"
			  "}",
			  sf::Shader::Fragment);
  _bgShader.setParameter("texture", _spriteSheet);
}

void TileCodex::applySpriteUV(const unsigned id, sf::Vertex *quad) const
{
  for (int i = 0; i < 4; ++i) {
    const sf::Vector2f& a = _spriteUVs.at(id * 4 + i);
    quad[i].texCoords = {a.x, a.y};
  }
}
