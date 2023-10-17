#include "crafting.h"

void CraftingManager::update_texture(void)
{
	bool large = m_number_ingredients + m_number_products > 4;

	// SPRITES

	m_rend_texture = new sf::RenderTexture();
	m_rend_texture->create(135, large ? 45 : 27);
	sf::Sprite sprite;
	sprite.setTexture(m_textures->get_texture("craft", large ? 1 : 0));
	if (!m_mouse_on)
		sprite.setColor(sf::Color(255, 255, 255, 127));
	m_rend_texture->draw(sprite);
	// Products
	int real_item_count = 0;
	for (int i = 0; i < 4; i++)
	{
		ItemStack product = RecipesList[m_recipe_number].products[i];
		if (product.get_item() != ITEM_NOTHING)
		{
			sf::Sprite new_sprite;
			new_sprite.setTexture(m_textures->get_texture(product.get_properties().texture_name, 0));
			new_sprite.setPosition(113 - (float)real_item_count * 16.f, large ? 23.f : 5.f);
			real_item_count++;
			m_rend_texture->draw(new_sprite);
			m_item_texts[i].setString(std::to_string(product.get_number()));
		}
	}
	// Arrow
	sf::Sprite arrow_sprite;
	arrow_sprite.setTexture(m_textures->get_texture("arrow", 0));
	arrow_sprite.setPosition(105 - (float)real_item_count * 16.f, large ? 23.f : 5.f);
	// Ingredients
	real_item_count = large ? 0 : real_item_count + 2;
	for (int i = 0; i < 4; i++)
	{
		ItemStack ingredient = RecipesList[m_recipe_number].ingredients[i];
		if (ingredient.get_item() != ITEM_NOTHING)
		{
			sf::Sprite new_sprite;
			new_sprite.setTexture(m_textures->get_texture(ingredient.get_properties().texture_name, 0));
			new_sprite.setPosition(113 - (float)real_item_count * 16.f, 5);
			real_item_count++;
			m_rend_texture->draw(new_sprite);
			m_item_texts[i + 4].setString(std::to_string(ingredient.get_number()));
		}
	}
	m_rend_texture->draw(arrow_sprite);
	m_rend_texture->display();
	// Text
	for (int i = 0; i < 8; i++)
	{
		m_item_texts[i].setFont(m_textures->get_font("default"));
		m_item_texts[i].setCharacterSize(12);
	}
}

std::vector<int> CraftingManager::get_available_crafts(Inventory* player_inventory)
{	
	std::vector<int> available_crafts;
	for (int i = 0; i < RECIPES_AMOUNT; i++)
		if (amount_available(player_inventory, i) > 0)
			available_crafts.push_back(i);
	return available_crafts;
}

int CraftingManager::amount_available(Inventory* player_inventory, int craft_number)
{
	ItemStack ingredients_count[4];
	for (int i1 = 0; i1 < 4; i1++)
		ingredients_count[i1] = ItemStack(RecipesList[craft_number].ingredients[i1].get_item(), 0);
	for (int i1 = 0; i1 < INVENTORY_SIZE; i1++)
	{
		ItemStack inv_item = player_inventory->get_items(i1);
		for (int i2 = 0; i2 < 4; i2++)
			ingredients_count[i2].add_items(inv_item, true);
	}
	bool not_max = true;
	int multiplicator = 0;
	while (not_max)
	{	
		multiplicator++;
		for (int i1 = 0; i1 < 4; i1++)
		{
			if (ingredients_count[i1].get_item() != ITEM_NOTHING && ingredients_count[i1].get_number() < RecipesList[craft_number].ingredients[i1].get_number() * multiplicator)
				not_max = false;
		}
	}
	return multiplicator - 1;
}

CraftingManager::CraftingManager(TextureResources* textures, Inventory* player_inventory, int recipe_number)
{	
	m_textures = textures;
	m_recipe_number = recipe_number;
	m_player_inventory = player_inventory;
	m_mouse_on = false;
	m_fading = Utils::TimeCounter(500);
	// Count items
	m_number_ingredients = 0;
	m_number_products = 0;
	for (int i = 0; i < 4; i++)
	{
		if (RecipesList[m_recipe_number].ingredients[i].get_item() != ITEM_NOTHING)
			m_number_ingredients++;
		if (RecipesList[m_recipe_number].products[i].get_item() != ITEM_NOTHING)
			m_number_products++;
	}
	// Texture
	update_texture();
}

bool CraftingManager::click(bool right)
{	
	if (m_mouse_on && !m_fading.started())
	{	
		int nb_crafts = amount_available(m_player_inventory, m_recipe_number);
		for (int i = 0; i < (right ? nb_crafts : 1); i++)
		{
			for (int i = 0; i < 4; i++) // Remove ingredients
			{
				ItemStack ingredient = RecipesList[m_recipe_number].ingredients[i];
				if (ingredient.get_item() != ITEM_NOTHING)
					m_player_inventory->remove_items(ingredient);
			}
		}
		for (int i = 0; i < (right ? nb_crafts : 1); i++)
		{
			for (int i = 0; i < 4; i++) // Add products
			{
				ItemStack product = RecipesList[m_recipe_number].products[i];
				if (product.get_item() != ITEM_NOTHING)
					m_player_inventory->add_items(product);
			}
		}
		return true;
	}
	return false;
}

float CraftingManager::update_and_draw(sf::RenderWindow& window, sf::Vector2f transform_ratio, sf::Vector2f mouse_pos, sf::Vector2f pos)
{	
	int craft_amount = amount_available(m_player_inventory, m_recipe_number);
	// Fading
	sf::Color fading_color(255, 255, 255);
	if (m_fading.started())
	{
		if (craft_amount > 0)
			m_fading.reset();
		else
			fading_color.a = (int)((1 - m_fading.progress()) * 255);
	}
	else if (craft_amount == 0)
		m_fading.start();
	// Craft block
	sf::Sprite sprite;
	sprite.setTexture(m_rend_texture->getTexture());
	sprite.setScale(transform_ratio * 2.f);
	sprite.setColor(fading_color);
	sprite.setPosition(pos);

	if (sprite.getGlobalBounds().contains(mouse_pos) != m_mouse_on)
	{	
		m_mouse_on = !m_mouse_on;
		update_texture();
	}
	window.draw(sprite);
	// Items text
	for (int i = 0; i < 8; i++)
	{
		if (m_item_texts[i].getString() != '0')
		{	
			sf::Vector2f text_pos;
			if (m_number_ingredients + m_number_products > 4)
			{	
				text_pos.x = (224 - (i % 4) * 32) * transform_ratio.x + pos.x;
				text_pos.y = (i < 4 ? 42 : 6) * transform_ratio.y + pos.y;
			}
			else
			{	
				if (i > 3) // Ingredient
					text_pos.x = (224 - (i - 2 + m_number_products) * 32) * transform_ratio.x + pos.x;
				else
					text_pos.x = (224 - (i % 4) * 32) * transform_ratio.x + pos.x;
				text_pos.y = 6 * transform_ratio.y + pos.y;
			}
			m_item_texts[i].setPosition(text_pos);
			m_item_texts[i].setScale(transform_ratio);
			m_item_texts[i].setFillColor(fading_color);
			window.draw(m_item_texts[i]);
		}
	}
	// Amount text
	sf::Text amount_text;
	amount_text.setFont(m_textures->get_font("title"));
	amount_text.setCharacterSize(20);
	std::string nb_txt = std::to_string(craft_amount);
	amount_text.setString(nb_txt);
	amount_text.setScale(transform_ratio);
	amount_text.setPosition(sf::Vector2f((33 - 12 * nb_txt.length() * transform_ratio.x) * transform_ratio.x, 12 * transform_ratio.y) + pos);
	amount_text.setFillColor(fading_color);
	window.draw(amount_text);

	return pos.y + m_rend_texture->getSize().y * 2;
}

bool CraftingManager::is_terminated(void) const
{
	return m_fading.finished();
}

bool CraftingManager::is_mouse_in_ui(void) const
{
	return m_mouse_on;
}

int CraftingManager::get_recipe(void) const
{
	return m_recipe_number;
}
