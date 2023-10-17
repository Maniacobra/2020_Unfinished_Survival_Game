#include "displayer.h"

Chunk::Chunk(sf::Vector2i pos)
{	
	m_chunk_pos = pos;
	pos *= CHUNKS_SIZE;
	m_rect = sf::IntRect(pos, sf::Vector2i(CHUNKS_SIZE, CHUNKS_SIZE));
	m_texture.create(CHUNKS_SIZE * SPRITE_SIZE, CHUNKS_SIZE * SPRITE_SIZE);
	m_sprite.setPosition(sf::Vector2f((float)(m_rect.left - MAP_MAX_SIZE / 2), (float)(m_rect.top - MAP_MAX_SIZE / 2)) * (float)SPRITE_SIZE);
}

bool Chunk::add_square(Square* square)
{	
	sf::Vector2i pos(square->get_pos());
	if (m_rect.contains(pos))
	{	
		square->set_chunk_pos(m_chunk_pos);
		m_squares.push_back(square);
		return true;
	}
	return false;
}

void Chunk::display(void)
{	
	for (auto it(m_squares.begin()); it != m_squares.end(); ++it)
		(*it)->draw(m_texture);
	m_texture.display();
	m_sprite.setTexture(m_texture.getTexture());
}

void Chunk::draw(sf::RenderWindow& window)
{	
	window.draw(m_sprite);
}

void Chunk::move(sf::Vector2f direction)
{
	m_sprite.move(direction);
}

void Chunk::draw_square(Square* square)
{	
	square->draw(m_texture);
	m_texture.display();
	m_sprite.setTexture(m_texture.getTexture());
	m_drawed_squares++;
}

bool Chunk::is_inWindow(sf::FloatRect draw_zone) const
{
	if (draw_zone.intersects(m_sprite.getGlobalBounds()))
		return true;
	return false;
}

sf::Vector2i Chunk::get_pos(void) const
{
	return sf::Vector2i(m_rect.left, m_rect.top);
}

sf::Vector2i Chunk::get_chunk_pos(void) const
{
	return m_chunk_pos;
}

int Chunk::get_nb_squares(void) const
{
	return m_squares.size();
}

std::vector<Square*> Chunk::get_squares(void) const
{
	return m_squares;
}
