#include "blocks.h"

void Block::init(BlockType id, TextureResources& textures, Block* neighbors[8], BlockOrientation rot, bool placed_by_player, std::vector<float> params)
{	
	m_id = id;
	if (placed_by_player)
		m_revealed = true;
	pre_init(params);
	if (get_properties().texture_name != "")
		m_textures = textures.get_group(get_properties().texture_name);
	if (get_properties().rand_texture)
		m_selected_texture = rand() % m_textures.size();
	m_rot = rot;
	for (int i = 0; i < 8; i++)
		if (neighbors[i] != nullptr)
		{
			m_connexions[i] = neighbors[i];
			m_connexions[i]->add_interaction(this, Utils::swapside(i));
		}
	post_init(params);
}

Block::Block(void)
{	
	m_connexions[0] = nullptr;
	m_connexions[1] = nullptr;
	m_connexions[2] = nullptr;
	m_connexions[3] = nullptr;
	m_connexions[4] = nullptr;
	m_connexions[5] = nullptr;
	m_connexions[6] = nullptr;
	m_connexions[7] = nullptr;
	m_covered = false;
	m_id = BLOCK_VOID;
	m_rot = TOP;
	m_undrawed = false;
	m_selected_texture = 0;
	m_revealed = false;
}

void Block::destroy(void)
{	
	on_destroy();
	for (int i = 0; i < 4; i++)
		if (m_connexions[i] != nullptr)
		{
			m_connexions[i]->remove_interaction(this, Utils::swapside(i));
			m_connexions[i] = nullptr;
		}
}

void Block::add_interaction(Block* block, BlockOrientation side)
{
	m_connexions[side] = block;
	on_interaction_add(side);
}

void Block::remove_interaction(Block* block, BlockOrientation side)
{
	if (m_connexions[side] != block)
		assert("La connexion n'a pas pu être retirée");
	on_interaction_remove(side);
	m_connexions[side] = nullptr;
}

void Block::draw(sf::Sprite& sprite)
{	
	sprite.setRotation((float)m_rot * 90.f);
	sprite.setOrigin(Utils::origin(m_rot));
	if (m_textures[m_selected_texture] != nullptr)
		sprite.setTexture(*m_textures[m_selected_texture]);
}

void Block::switch_cover(void)
{
	m_covered = !m_covered;
}

Block* Block::get_connexion(int side)
{
	return m_connexions[side];
}

BlockOrientation Block::get_rot(void) const
{
	return m_rot;
}

BlockType Block::get_id(void) const
{
	return m_id;
}

BlockProperties Block::get_properties(void) const
{
	return BlockPropertiesList[m_id];
}

int Block::get_solid_connexions(void) const
{
	int nb = 0;
	for (int i = 0; i < 8; i++)
	{
		if (m_connexions[i] != nullptr && m_connexions[i]->get_properties().solid)
			nb += (int)(pow(2, i));
	}
	return nb;
}

bool Block::is_isolated(void)
{	
	if (m_revealed)
		return false;
	for (int i = 0; i < 8; i++)
	{
		if (m_connexions[i] != nullptr && !m_connexions[i]->get_properties().solid)
		{	
			m_revealed = true;
			return false;
		}
	}
	return true;
}

bool Block::is_undrawed(void) const
{
	return m_undrawed;
}

bool Block::is_covered(void) const
{
	return m_covered;
}
