#pragma once
#ifndef BLOCKS_H_INCLUDED
#define BLOCKS_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "block_properties.h"
#include "res.h"
#include "utils.h"

class Block
{
protected:

	BlockType m_id;
	Block* m_connexions[8];
	TextureGroup m_textures;
	int m_selected_texture;
	BlockOrientation m_rot;
	bool m_undrawed;
	bool m_covered;
	bool m_revealed;
	// Methods
	virtual void pre_init(std::vector<float> params) {};
	virtual void post_init(std::vector<float> params) {};
	virtual void on_destroy() {};
	virtual void on_interaction_add(BlockOrientation side) {};
	virtual void on_interaction_remove(BlockOrientation side) {};

public:

	Block(void);
	virtual void init(BlockType id, TextureResources& textures, Block* neighbors[8], BlockOrientation rot, bool placed_by_player, std::vector<float> params);
	virtual void destroy(void);
	virtual void add_interaction(Block* block, BlockOrientation side);
	virtual void remove_interaction(Block* block, BlockOrientation side);
	virtual void update(void) {};
	virtual void post_update(void) {};
	virtual void edit(std::vector<float> params) {};
	virtual void draw(sf::Sprite& sprite);
	virtual void switch_cover(void);
	Block* get_connexion(int side);
	virtual bool is_isolated(void);
	virtual ~Block(void) {};
	// Const
	virtual std::vector<float> get_param(void) const { return std::vector<float>(); };
	virtual BlockOrientation get_rot(void) const;
	virtual BlockType get_id(void) const;
	virtual BlockProperties get_properties(void) const;
	virtual int get_solid_connexions(void) const;
	virtual bool is_undrawed(void) const;
	virtual bool is_covered(void) const;
	virtual bool is_active(void) const { return false; };
	virtual bool is_ticked(void) const { return false; };
};

class Grass : public Block
{
	sf::Color m_color;

public:

	Grass(void);
	virtual void post_init(std::vector<float> params);
	virtual void draw(sf::Sprite& sprite);
	virtual ~Grass(void) {};
};

#endif // !BLOCKS_H_INCLUDED
