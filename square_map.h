#pragma once
#ifndef SQUARE_MAP_H_INCLUDED
#define SQUARE_MAP_H_INCLUDED

#include <list>
#include <map>
#include <set>
#include <SFML/Graphics.hpp>

#include "square.h"

class SquareMap
{
	TextureResources* m_textures;
	// Blocks
	std::list<Square*> m_all_squares;
	std::list<Square*> m_active_squares;
	std::list<Block*> m_ticked_blocks;
	std::set<Square*> m_toDraw;
	std::map<sf::Vector2i, Square*, Utils::Vec2i_predicate> m_grid;

public:

	SquareMap();
	SquareMap(TextureResources& textures);
	void init_new_square(sf::Vector2i pos, Square* new_square);
	void tick_update(void);
	ItemStack place_block(Square* square, BlockType id, bool top = true, bool placed_by_player = false, BlockOrientation rot = TOP, std::vector<float> params = std::vector<float>());
	void rot_block(Square* square, bool clockwise);
	std::set<Square*> get_toDraw(void);
	Square* get_square(sf::Vector2f pos);
	Square* get_square(int x, int y);
	// Reset
	void restart();
};

#endif // !SQUARE_MAP_H_INCLUDED
