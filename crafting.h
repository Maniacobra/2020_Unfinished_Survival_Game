#pragma once
#ifndef CRAFTING_H_INCLUDED
#define CRAFTING_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

#include "inventory.h"
#include "items.h"
#include "res.h"

const int RECIPES_AMOUNT = 4;

struct CraftingRecipe
{
	ItemStack ingredients[4];
	ItemStack products[4];
};

static const CraftingRecipe RecipesList[RECIPES_AMOUNT]
{
	{{ItemStack(ITEM_ROCK, 8)}, {ItemStack(ITEM_COBBLESTONE, 1)}}, // COBBLE
	{{ItemStack(ITEM_LOG, 2), ItemStack(ITEM_STICK, 3)}, {ItemStack(ITEM_LOGS, 1)}}, // LOGS
	{{ItemStack(ITEM_STICK, 2), ItemStack(ITEM_ROCK, 3)}, {ItemStack(ITEM_PICKAXE, 1)}}, // PICKAXE
	{{ItemStack(ITEM_PICKAXE, 1)}, {ItemStack(ITEM_STICK, 1), ItemStack(ITEM_ROCK, 2)}}, // Decraft PICKAXE
};

class CraftingManager
{	
	TextureResources* m_textures;
	int m_recipe_number;
	int m_number_ingredients;
	int m_number_products;
	bool m_mouse_on;
	Inventory* m_player_inventory;
	sf::RenderTexture* m_rend_texture;
	Utils::TimeCounter m_fading;
	// Text
	sf::Text m_item_texts[8];
	// Methods
	void update_texture(void);

public:

	static std::vector<int> get_available_crafts(Inventory* player_inventory);
	static int amount_available(Inventory* player_inventory, int craft_number);

	CraftingManager(TextureResources* textures, Inventory* player_inventory, int recipe_number);
	bool click(bool right);
	float update_and_draw(sf::RenderWindow& window, sf::Vector2f transform_ratio, sf::Vector2f mouse_pos, sf::Vector2f pos);
	// Const
	bool is_terminated(void) const;
	bool is_mouse_in_ui(void) const;
	int get_recipe(void) const;
};

#endif // !CRAFTING_H_INCLUDED
