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
    Ground,
    Vine,
    Grass,
    Plank,
    DStep,
    Empty1,
    Brick,
    Tnt,
    Empty2,
    Empty3,
    SWeb,
    Rflower,
    Yflower,
    Empty4,
    Empty5,
    Rock,
    Obsidian,
    Sand,
    Gravel,
    Wood
    };

#endif /* TILETYPE_H */
