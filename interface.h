#pragma once
#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <set>
#include <SFML/Graphics.hpp>
#include <string>

#include "block_properties.h"
#include "crafting.h"
#include "inventory.h"
#include "res.h"
#include "utils.h"

class InventoryMenu
{
	TextureResources* m_textures;
	Inventory* m_player_inventory;
	bool m_open;
	Utils::TimeCounter m_counter;
	int m_inv_selected_slot;
	// Render textures
	sf::RenderTexture* m_inv_texture;
	// Crafting
	std::set<CraftingManager*> m_available_crafts;
	// Text
	sf::Text m_inventory_text;

public:

	InventoryMenu(void);
	InventoryMenu(TextureResources* textures, Inventory* player_inventory);
	bool click(bool right, int selected_slot);
	void toggle(void);
	void update_and_draw(sf::RenderWindow& window, sf::Vector2f transform_ratio, sf::Vector2f mouse_pos);
	// Const
	bool is_mouse_in_ui(void) const;
};

class InterfaceManager
{	
	TextureResources* m_textures;
	// Inventory
	Inventory* m_player_inventory;
	InventoryMenu m_inv_menu;
	ItemType m_selected_item_type;
	// Sprites & Text
	sf::Sprite m_selected_item;
	sf::Sprite m_toolbar;
	sf::Sprite m_selection_square;
	sf::Text m_fps_text;
	sf::Text m_chunks_text;
	// Div
	int m_current_selection;
	Utils::TimeCounter m_alpha;

public:

	InterfaceManager(TextureResources* textures, Inventory* player_inventory);
	bool click(bool right);
	void change_selection(int toolbar_case);
	void scroll(bool foward);
	void pick_block(void);
	void toggle_inv_menu(void);
	void update_and_draw(sf::RenderWindow& window, sf::Vector2f win_mouse_pos, sf::Vector2f map_mouse_pos, int fps, int nb_chunks);
	// Const
	bool is_mouse_in_ui(void) const;
	BlockType get_selected_material(void) const;
};

#endif // !INTERFACE_H_INCLUDED
