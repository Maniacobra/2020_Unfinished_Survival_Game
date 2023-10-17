#include "entity.h"

Player::Player(SquareMap* square_map, TextureGroup player_texture_group, sf::Vector2i pos)
{	
	m_square_map = square_map;
	m_animation = Animation(player_texture_group, true, 5);
	m_square_pos = sf::Vector2<double>(pos.x, pos.y);
	m_inventory = Inventory();
	update_sprite();
	m_counter = Utils::TimeCounter(1000);
	m_counter.start();
	// Stats
	m_hp = 400;
	m_max_hp = 400;
}

void Player::update(void)
{
	m_animation.update();
	update_sprite();
}

void Player::nonticked_update(void)
{	
	// Movement
	float speed = (float)(m_counter.progress() * PLAYER_SPEED);
	m_direction.x = 0;
	m_direction.y = 0;
	sf::Vector2f old_pos;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		m_direction.x = -1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_direction.x += 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		m_direction.y = -1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_direction.y += 1;
	if (m_direction.x != 0)
		m_square_pos.x += (double)speed * (m_direction.x > 0 ? 1 : -1);
	if (m_direction.y != 0)
		m_square_pos.y += (double)speed * (m_direction.y > 0 ? 1 : -1);
	// Rotate
	if (m_direction.x == 0 && m_direction.y == -1)
		m_sprite.setRotation(0);
	if (m_direction.x == 1 && m_direction.y == -1)
		m_sprite.setRotation(45);
	if (m_direction.x == 1 && m_direction.y == 0)
		m_sprite.setRotation(90);
	if (m_direction.x == 1 && m_direction.y == 1)
		m_sprite.setRotation(135);
	if (m_direction.x == 0 && m_direction.y == 1)
		m_sprite.setRotation(180);
	if (m_direction.x == -1 && m_direction.y == 1)
		m_sprite.setRotation(225);
	if (m_direction.x == -1 && m_direction.y == 0)
		m_sprite.setRotation(270);
	if (m_direction.x == -1 && m_direction.y == -1)
		m_sprite.setRotation(315);
	// Anim
	if (m_direction.x == 0 && m_direction.y == 0)
		m_animation.stop(2);
	else if (!m_animation.is_running())
		m_animation.run(3);
	// Update
	collision_update();
	update_sprite();
	// Counter
	m_counter.start();
}

void Player::give_items(ItemStack items)
{
	m_inventory.add_items(items);
}

Inventory* Player::get_inventory(void)
{
	return &m_inventory;
}

void Player::draw(sf::RenderWindow& window) const
{	
	window.draw(m_sprite);
}
