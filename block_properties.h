#pragma once
#ifndef BLOCK_PROPERTIES_H_INCLUDED
#define BLOCK_PROPERTIES_H_INCLUDED

#include "items.h"

enum BlockOrientation { TOP, RIGHT, BOT, LEFT };

struct BlockProperties
{
	std::string texture_name;
	bool solid;
	bool rand_texture;
	ItemType drop;
	int drop_count;
};

static const struct BlockProperties BlockPropertiesList[] =
{	
	{"", false, false, ITEM_NOTHING, 0}, // VOID
	{"", false, false, ITEM_NOTHING, 0}, // AIR
	{"cobblestone", true, false, ITEM_COBBLESTONE, 1}, // COBBLE
	{"grass", true, false, ITEM_NOTHING, 0}, // GRASS
	{"logs", true, false, ITEM_LOGS, 1}, // LOGS
	{"sand", true, false, ITEM_NOTHING, 0}, // SAND
	{"sticks_on_ground", false, true, ITEM_STICK, 1}, // STICKS
	{"stone", true, false, ITEM_ROCK, 5}, // STONE
	{"water", false, false, ITEM_NOTHING, 0} // WATER
};

#endif // !BLOCK_PROPERTIES_H_INCLUDED
