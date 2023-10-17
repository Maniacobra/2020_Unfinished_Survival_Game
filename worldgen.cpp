#include "worldgen.h"

WorldGenerator::WorldGenerator(void)
{
	m_seed = 0;
}

WorldGenerator::WorldGenerator(unsigned int seed)
{	
	m_seed = seed;
	if (m_seed == 0)
	{	
		long current_time = time(NULL);
		srand(current_time);
		logw("\nGenerator created with random seed : " + std::to_string(current_time));
	}
	else
	{
		srand(m_seed);
		logw("\nGenerator created with set seed : " + std::to_string(seed));
	}
	for (int i = 0; i < WORLD_STRETCHING_NB_PARAMETERS; i++)
		m_perlin_noises[i] = PerlinNoise(rand());
	m_population_pns[0] = PerlinNoise(rand());
	srand(time(NULL));
}

GenResult WorldGenerator::generate(int int_x, int int_y)
{	
	double x;
	double y;
	double noise;
	double value = 0;
	for (int i = 0; i < WORLD_STRETCHING_NB_PARAMETERS; i++)
	{
		x = (double)int_x / WORLD_STRETCHING_VALUES[i];
		y = (double)int_y / WORLD_STRETCHING_VALUES[i];
		noise = m_perlin_noises[i].noise(x, y, 1);
		value += noise * pow(noise + 0.5, WORLD_STRETCHING_EXTREMES[i]);
	}
	value /= WORLD_STRETCHING_NB_PARAMETERS;
	GenResult result;
	result.value = value;
	if (value < 0.28)
		result.square = GenResultBlock::WATER;
	else if (value < 0.295)
		result.square = GenResultBlock::SAND;
	else if (value < 0.41)
	{	
		x = (double)int_x / 2;
		y = (double)int_y / 2;
		noise = m_population_pns[0].noise(x, y, 1);
		if (noise < 0.35)
			result.square = GenResultBlock::GRASS_AND_STICKS;
		else if (noise < 0.75)
			result.square = GenResultBlock::GRASS;
		else
			result.square = GenResultBlock::TREE;
	}
	else if (value < 0.5)
		result.square = GenResultBlock::GRASS;
	else if (value < 0.51)
		result.square = GenResultBlock::STONE;
	else
		result.square = GenResultBlock::MOUNTAIN;
	return result;
}

