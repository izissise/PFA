#include <stdexcept>

#include "TileCodex.hpp"

const unsigned TileCodex::tileSize;

TileCodex::TileCodex(const std::string& path)
{
  // load sprite sheet
  if (not _spriteSheet.loadFromFile(path)) {
    throw std::invalid_argument("Invalid sprite sheet path");
  }

  // generate sprites
  sf::IntRect	rect(0, 0, TileCodex::tileSize, TileCodex::tileSize);
  _texSize.x = _spriteSheet.getSize().x;
  _texSize.y = _spriteSheet.getSize().y;

  for (rect.top = 0; rect.top < _texSize.y; rect.top += rect.width) {
    for (rect.left = 0; rect.left < _texSize.x; rect.left += rect.height) {
      _sprites.push_back(sf::Sprite(_spriteSheet, rect));
      _sprites.back().setPosition(0.0f, 0.0f);
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
			  "  gl_FragColor = gl_Color * pixel * vec4(0.7, 0.7, 0.7, 1.0);"
			  "}",
			  sf::Shader::Fragment);
  _bgShader.setParameter("texture", _spriteSheet);
}
