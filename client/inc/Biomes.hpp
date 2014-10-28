#ifndef _BIOMES_H_
#define _BIOMES_H_

enum class	Biome
{
  Ice,
    Forest,
    Desert,
    Prairie,
    Dry,
    Swamp
};

typedef struct	s_BiomeInfo
{
  Biome		name;
  int		minHeight;
  int		minHumidity;
  int		maxHumidity;
}		t_BiomeInfo;

#endif /* _BIOMES_H_ */
