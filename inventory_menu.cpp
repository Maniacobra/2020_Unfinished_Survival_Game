#include "interface.h"

InventoryMenu::InventoryMenu(void)
{
	m_textures = nullptr;
	m_player_inventory = nullptr;
	m_counter = Utils::TimeCounter(200);
	m_inv_texture = new sf::RenderTexture;
	m_open = false;
	m_inv_selected_slot = -1;
	m_inventory_text;
}

InventoryMenu::InventoryMenu(TextureResources* textures, Inventory* player_inventory)
{
	m_textures = textures;
	m_player_inventory = player_inventory;
	m_counter = Utils::TimeCounter(200);
	m_open = false;
	m_inv_selected_slot = -1;
	m_inv_texture = new sf::RenderTexture;
	m_inv_texture->create(61, 201);
	m_inventory_text.setFont(m_textures->get_font("title"));
	m_inventory_text.setCharacterSize(20);
	m_inventory_text.setString("Inventaire");
}

bool InventoryMenu::click(bool right, int selected_slot)
{	
	if (m_inv_selected_slot != -1)
	{	
		m_player_inventory->swap_items(m_inv_selected_slot, selected_slot);
		return true;
	}
	for (auto it = m_available_crafts.begin(); it != m_available_crafts.end(); ++it)
		if ((*it)->click(right))
			return true;
	return false;
}

void InventoryMenu::toggle(void)
{
	m_open = !m_open;
	m_counter.start();
}

void InventoryMenu::update_and_draw(sf::RenderWindow& window, sf::Vector2f transform_ratio, sf::Vector2f mouse_pos)
{	
	if (!m_open && (m_counter.finished() || !m_counter.started()))
		return;
	sf::Vector2f pos_start;
	pos_start.x = m_open ? (m_counter.finished() ? 0 : (1 - (float)m_counter.progress()) * -180.f) : (m_counter.finished() ? -180 : (float)m_counter.progress() * -180.f) * transform_ratio.x;
	pos_start.y = 350 - (float)m_inv_texture->getSize().y / 2.f * 3.f;
	
	// INVENTORY

	m_inv_texture->clear(sf::Color::Transparent);
	m_inventory_text.setPosition(pos_start.x + 12 * transform_ratio.x, pos_start.y - 24 * transform_ratio.y);
	m_inventory_text.setScale(transform_ratio);
	window.draw(m_inventory_text);
	sf::Vector2f inv_pos = pos_start + sf::Vector2f(10 * transform_ratio.x, 0);
	// Items
	sf::Text items_texts[INVENTORY_SIZE - 9];
	m_inv_selected_slot = -1;
	for (int i = 9; i < INVENTORY_SIZE; i++)
	{
		// Case
		sf::Vector2f pos;
		pos.x = (float)(i % 3 * 20 + 2);
		pos.y = (float)((i - 9) / 3 * 20 + 2);
		sf::Sprite case_sprite;
		case_sprite.setTexture(m_textures->get_texture("inv_case", 0));
		case_sprite.setPosition(pos - sf::Vector2f(2, 2));
		if (!case_sprite.getGlobalBounds().contains((mouse_pos.x - inv_pos.x) / 3.f / transform_ratio.x, (mouse_pos.y - inv_pos.y) / 3.f / transform_ratio.y))
			case_sprite.setColor(sf::Color(255, 255, 255, 127));
		else
			m_inv_selected_slot = i;
		m_inv_texture->draw(case_sprite);
		// Item
		if (m_player_inventory->get_items(i).get_item() == ITEM_NOTHING)
			continue;
		sf::Sprite item_sprite;
		item_sprite.setTexture(m_textures->get_texture(m_player_inventory->get_items(i).get_properties().texture_name, 0));
		item_sprite.setPosition(pos);
		m_inv_texture->draw(item_sprite);
		// Text
		sf::Text item_text;
		item_text.setFont(m_textures->get_font("default"));
		item_text.setCharacterSize(16);
		item_text.setScale(transform_ratio);
		item_text.setString(std::to_string(m_player_inventory->get_items(i).get_number()));
		sf::Vector2f item_text_pos;
		item_text_pos.x = case_sprite.getPosition().x * transform_ratio.x * 3.f + pos_start.x + 13 * transform_ratio.x;
		item_text_pos.y = case_sprite.getPosition().y * transform_ratio.y * 3.f + pos_start.y;
		item_text.setPosition(item_text_pos);
		items_texts[i - 9] = item_text;
	}
	sf::Sprite inv_sprite;
	m_inv_texture->display();
	inv_sprite.setTexture(m_inv_texture->getTexture());
	inv_sprite.setPosition(inv_pos);
	inv_sprite.setScale(transform_ratio * 3.f);
	window.draw(inv_sprite);
	for (int i = 0; i < INVENTORY_SIZE - 9; i++)
		window.draw(items_texts[i]);

	// CRAFTING MENU

	// Delete
	std::vector<CraftingManager*> to_delete;
	for (auto it = m_available_crafts.begin(); it != m_available_crafts.end(); ++it)
		if ((*it)->is_terminated())
			to_delete.push_back(*it);
	for (auto it = to_delete.begin(); it != to_delete.end(); ++it)
	{
		m_available_crafts.erase(*it);
		delete* it;
	}
	// Insert
	std::vector<int> available_crafts_result = CraftingManager::get_available_crafts(m_player_inventory);
	for (auto it1 = available_crafts_result.begin(); it1 != available_crafts_result.end(); ++it1)
	{
		bool already_here = false;
		for (auto it2 = m_available_crafts.begin(); it2 != m_available_crafts.end(); ++it2)
			if ((*it2)->get_recipe() == *it1)
				already_here = true;
		if (!already_here)
			m_available_crafts.insert(new CraftingManager(m_textures, m_player_inventory, *it1));
	}
	// Update and draw
	sf::Vector2f pos = sf::Vector2f(pos_start);
	for (auto it = m_available_crafts.begin(); it != m_available_crafts.end(); ++it)
		pos.y = (*it)->update_and_draw(window, transform_ratio, mouse_pos, sf::Vector2f(WIN_X - (pos_start.x + 180 * transform_ratio.x) * 1.5f, pos.y));
}

bool InventoryMenu::is_mouse_in_ui(void) const
{	
	if (m_inv_selected_slot != -1)
		return true;
	for (auto it = m_available_crafts.begin(); it != m_available_crafts.end(); ++it)
		if ((*it)->is_mouse_in_ui())
			return true;
	return false;
}
