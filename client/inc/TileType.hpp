#ifndef	TILETYPE_H
# define TILETYPE_H

/*
** TileType is currently defined as an 8-bit identifier
** It should be noted that it must be changed if
** over 256 different tile types are needed.
*/
enum class TileType : std::int8_t
{
  Empty,
    Ground
    };

#endif /* TILETYPE_H */
