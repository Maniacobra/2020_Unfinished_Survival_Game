#pragma once
#ifndef ENTITY_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "animation.h"
#include "inventory.h"
#include "program_const.h"
#include "res.h"
#include "square.h"
#include "square_map.h"
#include "utils.h"

class Entity
{
protected:

	Animation m_animation;
	sf::Sprite m_sprite;
	sf::Vector2<double> m_square_pos;
	bool m_top_layer;
	SquareMap* m_square_map;
	// Stats
	ItemStack m_loot;
	int m_max_hp;
	int m_hp;
	// Methods
	virtual void collision_update(void);
	virtual void update_sprite(void);
	virtual void on_death(void) {};

public:


	Entity(void)
	{	
		m_square_map = nullptr;
		m_top_layer = false;
		m_hp = 200;
		m_max_hp = 200;
	};
	virtual void update(void) {};
	virtual ~Entity(void) {};
	virtual void hit(Entity* source, int dmg);
	// Const
	virtual void draw(sf::RenderWindow& window) const {};
	virtual void draw_hp_bar(sf::RenderWindow& window) const;
	virtual sf::Vector2<double> get_square_pos(void) const;
	virtual bool is_mouseOn(sf::Vector2f mouse_pos) const;
	virtual bool is_inWindow(sf::FloatRect draw_zone) const;
	virtual ItemStack get_loot(void) const
	{
		return m_loot;
	}
	virtual bool is_dead(void) const
	{
		return m_hp <= 0;
	}
	virtual bool is_top_layer(void) const
	{
		return m_top_layer;
	}
};

class Player : public Entity
{
	sf::Vector2i m_direction;
	Utils::TimeCounter m_counter;
	Inventory m_inventory;

public:

	Player(void) {};
	Player(SquareMap* square_map, TextureGroup player_texture_group, sf::Vector2i pos);
	virtual void update(void);
	void nonticked_update(void);
	void give_items(ItemStack items);
	Inventory* get_inventory(void);
	virtual void draw(sf::RenderWindow& window) const;
	virtual ~Player(void) {};
};

class Tree : public Entity
{	
	virtual void on_death(void);

public:

	Tree(void) {};
	Tree(SquareMap* square_map, TextureGroup tree_texture_group, sf::Vector2i pos);
	virtual void draw(sf::RenderWindow& window) const;
	virtual ~Tree(void) {};
};

#endif // !ENTITY_H_INCLUDED
