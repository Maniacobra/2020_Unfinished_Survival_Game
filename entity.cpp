#include "entity.h"

void Entity::collision_update(void)
{	
	Square* associated_square = m_square_map->get_square((int)m_square_pos.x, (int)m_square_pos.y);
	if (associated_square == nullptr)
		return;
	// Left
	Block* temp_block = associated_square->get_block_top()->get_connexion(7);
	if (temp_block == nullptr || temp_block->get_properties().solid)
	{
		if (m_square_pos.x < associated_square->get_pos().x + 0.5)
			m_square_pos.x = associated_square->get_pos().x + 0.5;
	}
	// Top
	temp_block = associated_square->get_block_top()->get_connexion(1);
	if (temp_block == nullptr || temp_block->get_properties().solid)
	{	
		if (m_square_pos.y < associated_square->get_pos().y + 0.5)
			m_square_pos.y = associated_square->get_pos().y + 0.5;
	}
	// Right
	temp_block = associated_square->get_block_top()->get_connexion(3);
	if (temp_block == nullptr || temp_block->get_properties().solid)
	{
		if (m_square_pos.x > associated_square->get_pos().x + 0.5)
			m_square_pos.x = associated_square->get_pos().x + 0.5;
	}
	// Bot
	temp_block = associated_square->get_block_top()->get_connexion(5);
	if (temp_block == nullptr || temp_block->get_properties().solid)
	{
		if (m_square_pos.y > associated_square->get_pos().y + 0.5)
			m_square_pos.y = associated_square->get_pos().y + 0.5;
	}
}

void Entity::update_sprite(void)
{	
	sf::Vector2f pixel_pos((float)(m_square_pos.x - MAP_MAX_SIZE / 2), (float)(m_square_pos.y - MAP_MAX_SIZE / 2));
	pixel_pos *= (float)SPRITE_SIZE;
	sf::Vector2f sprite_pos(pixel_pos.x, pixel_pos.y);
	const float PI = (float)(2 * acos(0.0));
	float angle_rad = m_sprite.getRotation() * PI / 180.f + PI / 4.f;
	float radius = sqrt(pow(m_sprite.getLocalBounds().width / 2, 2) + pow(m_sprite.getLocalBounds().height / 2, 2));
	sprite_pos.x -= cos(angle_rad) * radius;
	sprite_pos.y -= sin(angle_rad) * radius;
	m_sprite.setPosition(sprite_pos);
	m_sprite.setTexture(*m_animation.get_texture());
}

void Entity::hit(Entity* source, int dmg)
{
	m_hp -= dmg;
	if (m_hp <= 0)
		on_death();
}

void Entity::draw_hp_bar(sf::RenderWindow& window) const
{	
	if (m_hp == m_max_hp)
		return;
	sf::RectangleShape empty_bar;
	empty_bar.setSize(sf::Vector2f(20, 2));
	sf::Vector2f pos(m_sprite.getGlobalBounds().left + m_sprite.getGlobalBounds().width / 2 - 10, m_sprite.getGlobalBounds().top + m_sprite.getGlobalBounds().height / 2 - 1);
	empty_bar.setPosition(pos);
	empty_bar.setFillColor(sf::Color(127, 127, 127));
	window.draw(empty_bar);
	sf::RectangleShape hp_bar;
	hp_bar.setSize(sf::Vector2f((float)(20 * m_hp / m_max_hp), 2));
	hp_bar.setPosition(pos);
	hp_bar.setFillColor(sf::Color(0, 255, 0));
	window.draw(hp_bar);
}

sf::Vector2<double> Entity::get_square_pos(void) const
{
	return m_square_pos;
}

bool Entity::is_mouseOn(sf::Vector2f mouse_pos) const
{
	return m_sprite.getGlobalBounds().contains(mouse_pos);
}

bool Entity::is_inWindow(sf::FloatRect draw_zone) const
{	
	return m_sprite.getGlobalBounds().intersects(draw_zone);
}
