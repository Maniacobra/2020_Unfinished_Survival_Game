#pragma once
#ifndef WOLDGEN_H_INCLUDED

#include <time.h>

#include "log.h"
#include "PerlinNoise/PerlinNoise.h"
#include "program_const.h"
#include "square.h"

enum GenResultBlock
{
	GRASS,
	TREE,
	GRASS_AND_STICKS,
	SAND,
	WATER,
	MOUNTAIN,
	STONE
};

struct GenResult
{
	GenResultBlock square;
	double value;
};

class WorldGenerator
{
	unsigned int m_seed;
	PerlinNoise m_perlin_noises[WORLD_STRETCHING_NB_PARAMETERS];
	PerlinNoise m_population_pns[1];

public:

	WorldGenerator(void);
	WorldGenerator(unsigned int seed);
	GenResult generate(int int_x, int int_y);
};

#endif // !WOLDGEN_H_INCLUDED
