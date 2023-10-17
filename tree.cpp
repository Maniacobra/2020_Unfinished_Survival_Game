#include "entity.h"

void Tree::on_death(void)
{
	sf::Vector2i tree_pos((int)m_square_pos.x, (int)m_square_pos.y);
	for (int iy = tree_pos.y; iy != tree_pos.y + 3; iy++)
	{
		for (int ix = tree_pos.x; ix != tree_pos.x + 3; ix++)
		{
			Square* near_square = m_square_map->get_square(ix, iy);
			if (near_square != nullptr && near_square->get_block_top()->get_id() == BLOCK_AIR)
				m_square_map->place_block(near_square, BLOCK_STICKS);
		}
	}
}

Tree::Tree(SquareMap* square_map, TextureGroup tree_texture_group, sf::Vector2i pos)
{	
	m_square_map = square_map;
	m_animation = Animation(tree_texture_group, false, 0);
	m_animation.stop(0);
	m_square_pos = sf::Vector2<double>((double)pos.x - 1, (double)pos.y - 1);
	m_top_layer = true;
	update_sprite();
	// Stats
	m_loot = ItemStack(ITEM_LOG, 1);
	m_hp = 100;
	m_max_hp = 100;
}

void Tree::draw(sf::RenderWindow& window) const
{
	window.draw(m_sprite);
}
