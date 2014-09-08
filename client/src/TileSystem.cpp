#include "TileSystem.hpp"

TileSystem::TileSystem(unsigned width, unsigned height) :
  _map(width, height)
{
  _initBgShader();
}

TileSystem::TileSystem(unsigned width, unsigned height,
		       const std::string& path) :
  _map(width, height), _sheet(path)
{
  _initBgShader();
}

TileSystem::TileSystem(unsigned width, unsigned height,
		       const std::string& path, unsigned spriteSize) :
  _map(width, height), _sheet(path, spriteSize)
{
  _initBgShader();
}

void TileSystem::render(sf::RenderWindow& window)
{
  _renderLayer(window, &TileMap::getBgTile, &_bgShader);
  _renderLayer(window, &TileMap::getTile, nullptr);
}

void TileSystem::_renderLayer(sf::RenderWindow& window, TileMap::Getter getter, sf::Shader *shader)
{
  unsigned x;
  unsigned index;
  TileType tile;
  unsigned size = _sheet.getSize();
  unsigned maxX = _map.getWidth() * size;
  unsigned maxY = _map.getHeight() * size;

  index = 0;
  for (unsigned y = 0; y < maxY; y += size)
    {
      for (x = 0; x < maxX; x += size)
	{
	  if ((tile = (_map.*getter)(index)) != TileType::Empty)
	    {
	      sf::Sprite& tileSprite = _sheet.getSprite(static_cast<unsigned>(tile));

	      tileSprite.setPosition(x, y);
	      window.draw(tileSprite, shader);
	    }
	  ++index;
	}
    }
}

void TileSystem::_initBgShader(void)
{
  _bgShader.loadFromMemory("uniform sampler2D texture;"
			  ""
			  "void main()"
			  "{"
			  "  vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);"
			  "  gl_FragColor = gl_Color * pixel * vec4(0.7, 0.7, 0.7, 1.0);"
			  "}",
			  sf::Shader::Fragment);
  _bgShader.setParameter("texture", _sheet.getTexture());
}
