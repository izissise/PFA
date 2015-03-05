#include "coordinateSystemConversion.hpp"

#include "Vector2.hpp"

Vector2i physWorldToScreen(Vector2f const& camCenter, Vector2f const& objPos)
{
	//(chunkId + pos) * {Chunk::width, Chunk::height}
  return camCenter + (objPos);
}
