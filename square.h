#pragma once
#ifndef SQUARE_H_INCLUDED
#define SQUARE_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "blocks.h"
#include "utils.h"

class Square
{	
	sf::Vector2i m_pos;
	sf::Vector2f m_pixel_pos;
	sf::Vector2i m_chunk_pos;
	Block* m_block_top;
	Block* m_block_bot;
	bool m_highlighted;
	bool m_drawable;
	TextureGroup m_border_textures;

public:

	Square(sf::Vector2i grid_pos, TextureGroup m_border_textures);
	// Methods
	void set_chunk_pos(sf::Vector2i chunk_pos);
	void highlight(void);
	void unhighlight(void);
	void set_block_top(Block* block);
	void set_block_bot(Block* block);
	~Square();
	// Const
	void draw(sf::RenderTexture& chunk_texture);
	sf::Vector2i get_pos(void) const;
	sf::Vector2i get_chunk_pos(void) const;
	bool is_drawable(void) const;
	Block* get_block_top(void) const;
	Block* get_block_bot(void) const;
};

#endif // !SQUARE_H_INCLUDED