#include "blocks.h"

Grass::Grass(void)
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
	m_id = BLOCK_GRASS;
	m_rot = TOP;
	m_undrawed = false;
	m_selected_texture = 0;
	// Grass
	m_color = sf::Color::White;
}

void Grass::post_init(std::vector<float> params)
{	
	if (params.size() == 0)
		return;
	m_color.g = (int)(params[0] * 511.f);
	m_color.b = (int)(params[0] * 511.f);
}

void Grass::draw(sf::Sprite& sprite)
{
	sprite.setRotation((float)m_rot * 90.f);
	sprite.setOrigin(Utils::origin(m_rot));
	if (m_color != sf::Color::White)
		sprite.setColor(m_color);
	if (m_textures[m_selected_texture] != nullptr)
		sprite.setTexture(*m_textures[m_selected_texture]);
}
