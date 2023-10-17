#include "interface.h"

InterfaceManager::InterfaceManager(TextureResources* textures, Inventory* player_inventory)
{	
	m_textures = textures;
	// Inventory
	m_player_inventory = player_inventory;
	m_inv_menu = InventoryMenu(textures, player_inventory);
	// Toolbar
	m_toolbar.setTexture(m_textures->get_texture("toolbar", 0));
	m_alpha = Utils::TimeCounter(250);
	m_selection_square.setTexture(textures->get_texture("select_square", 0));
	m_current_selection = 0;
	m_selected_item_type = ITEM_NOTHING;
	// FPS
	m_fps_text.setFont(textures->get_font("default"));
	m_fps_text.setCharacterSize(15);
	m_chunks_text.setFont(textures->get_font("default"));
	m_chunks_text.setCharacterSize(15);
}

bool InterfaceManager::click(bool right)
{
	return m_inv_menu.click(right, m_current_selection);
}

void InterfaceManager::change_selection(int slot)
{	
	if (m_player_inventory->get_items(slot).get_item() == ITEM_NOTHING)
		m_alpha.reset();
	else
	{
		m_selected_item.setTexture(m_textures->get_texture(m_player_inventory->get_items(slot).get_properties().texture_name, 0));
		m_alpha.start();
	}
	m_current_selection = slot;
	m_selected_item_type = m_player_inventory->get_items(slot).get_item();
}

void InterfaceManager::scroll(bool foward)
{	
	if (foward)
	{	
		m_current_selection++;
		if (m_current_selection == 9)
			m_current_selection = 0;
	}
	else
	{
		m_current_selection--;
		if (m_current_selection == -1)
			m_current_selection = 8;
	}
	change_selection(m_current_selection);
}

void InterfaceManager::pick_block(void)
{	
	m_player_inventory->remove_items(m_current_selection, 1);
}

void InterfaceManager::toggle_inv_menu(void)
{
	m_inv_menu.toggle();
}

void InterfaceManager::update_and_draw(sf::RenderWindow& window, sf::Vector2f win_mouse_pos, sf::Vector2f map_mouse_pos, int fps, int nb_chunks)
{	
	// Ratio
	float smallest;
	sf::Vector2f transform_ratio((float)WIN_X / (float)window.getSize().x, (float)WIN_Y / (float)window.getSize().y);
	if (window.getSize().x < window.getSize().y)
		smallest = (float)window.getSize().x;
	else
		smallest = (float)window.getSize().y;
	transform_ratio *= smallest / 700.f;
	// Selected item
	if (m_player_inventory->get_items(m_current_selection).get_item() != m_selected_item_type)
		change_selection(m_current_selection);
	if (m_alpha.started())
	{
		m_selected_item.setColor(sf::Color(255, 255, 255, (int)((1 - m_alpha.progress()) * 100.f) + 155));
		float progress = (float)(m_alpha.progress());
		float resizing = 1;
		if (progress < 1)
			resizing = 1.5f - progress / 2.f;
		m_selected_item.setScale(sf::Vector2f(resizing, resizing));
		if (m_alpha.finished())
			m_alpha.reset();
	}
	sf::Vector2f item_pos = sf::Vector2f(map_mouse_pos);
	item_pos.x -= m_selected_item.getGlobalBounds().width / 2;
	item_pos.y -= m_selected_item.getGlobalBounds().height / 2;
	m_selected_item.setPosition(item_pos);
	if (m_selected_item_type != ITEM_NOTHING)
		window.draw(m_selected_item);
	// Toolbar
	window.setView(window.getDefaultView());
	m_toolbar.setScale(TOOLBAR_SIZE * transform_ratio.x, TOOLBAR_SIZE * transform_ratio.y);
	m_toolbar.setPosition(sf::Vector2f((float)WIN_X / 2.f - (float)m_toolbar.getGlobalBounds().width / 2.f, (float)WIN_Y - (float)m_toolbar.getGlobalBounds().height));
	window.draw(m_toolbar);
	for (int i = 0; i < 9; i++)
	{	
		if (m_player_inventory->get_items(i).get_item() == ITEM_NOTHING)
			continue;
		// Item
		sf::Sprite item_sprite;
		item_sprite.setTexture(m_textures->get_texture(m_player_inventory->get_items(i).get_properties().texture_name, 0));
		item_sprite.setPosition(sf::Vector2f(m_toolbar.getPosition().x + (float)m_toolbar.getGlobalBounds().width / 9.1f * i + (3.f * transform_ratio.x * TOOLBAR_SIZE), m_toolbar.getPosition().y + (3.f * transform_ratio.y * TOOLBAR_SIZE)));
		item_sprite.setScale(transform_ratio.x * TOOLBAR_SIZE, transform_ratio.y * TOOLBAR_SIZE);
		window.draw(item_sprite);
		// Text
		sf::Text item_text;
		item_text.setFont(m_textures->get_font("default"));
		item_text.setCharacterSize(15);
		item_text.setString(std::to_string(m_player_inventory->get_items(i).get_number()));
		item_text.setPosition(item_sprite.getPosition());
		item_text.setScale(transform_ratio);
		window.draw(item_text);
	}
	m_selection_square.setPosition(m_toolbar.getPosition().x + (float)m_toolbar.getGlobalBounds().width / 9.1f * m_current_selection - (3.f * transform_ratio.x), m_toolbar.getPosition().y - (3.f * transform_ratio.y));
	m_selection_square.setScale(TOOLBAR_SIZE * transform_ratio.x, TOOLBAR_SIZE * transform_ratio.y);
	window.draw(m_selection_square);
	// Inventory
	m_inv_menu.update_and_draw(window, transform_ratio, win_mouse_pos);
	// Text
	m_fps_text.setString(std::to_string(fps) + " FPS");
	m_fps_text.setScale(transform_ratio);
	m_chunks_text.setString(std::to_string(nb_chunks) + " chunks loading");
	m_chunks_text.setScale(transform_ratio);
	m_chunks_text.setPosition(0, 16 * transform_ratio.y);
	// Draw
	window.draw(m_fps_text);
	window.draw(m_chunks_text);
}

bool InterfaceManager::is_mouse_in_ui(void) const
{
	return m_inv_menu.is_mouse_in_ui();
}

BlockType InterfaceManager::get_selected_material(void) const
{
	return (BlockType)m_player_inventory->get_items(m_current_selection).get_properties().associated_block;
}
