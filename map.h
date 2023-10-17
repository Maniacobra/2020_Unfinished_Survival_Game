#pragma once
#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <set>
#include <SFML/Graphics.hpp>
#include <string>

#include "displayer.h"
#include "entity.h"
#include "log.h"
#include "res.h"
#include "square.h"
#include "square_map.h"
#include "utils.h"
#include "worldgen.h"

class GameMap
{	
	TextureResources* m_textures;
	// Squares
	WorldGenerator m_world_gen;
	SquareMap m_square_map;
	// Entities
	Player* m_player;
	std::set<Entity*> m_entities;
	// Div
	Utils::TimeCounter m_tick_counter;
	sf::Vector2i m_center_pos;
	Displayer* m_displayer;
	// Methods
	Square* init_square(Chunk* chunk);

public:

	GameMap(TextureResources& m_textures, Displayer* m_displayer);
	void update(void);
	bool hit_entity(sf::Vector2f mouse_pos, int dmg);
	// Squares
	ItemStack place_block(Square* square, BlockType id, bool top = true, bool placed_by_player = false, BlockOrientation rot = TOP, std::vector<float> params = std::vector<float>());
	bool hit_block(Square* square, bool top);
	void rot_block(Square* square, bool clockwise);
	Square* get_square(sf::Vector2f pos);
	// Entities
	void update_player(void);
	Player* get_player(void);
	void plant_tree(sf::Vector2i pos);
	// Reset
	void restart();
};

#endif // !MAP_H_INCLUDED
