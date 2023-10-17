#include "square.h"

Square::Square(sf::Vector2i grid_pos, TextureGroup border_textures)
{	
	m_pos = grid_pos;
	m_pixel_pos = sf::Vector2f(m_pos.x % CHUNKS_SIZE * (float)SPRITE_SIZE, m_pos.y % CHUNKS_SIZE * (float)SPRITE_SIZE);
	m_highlighted = false;
	m_drawable = false;
	m_border_textures = border_textures;
	m_block_bot = new Block();
	m_block_top = new Block();
}

void Square::set_chunk_pos(sf::Vector2i chunk_pos)
{
	m_chunk_pos = chunk_pos;
	m_drawable = true;
}

void Square::highlight(void)
{	
	m_highlighted = true;
}

void Square::unhighlight(void)
{
	m_highlighted = false;
}

void Square::set_block_top(Block* block)
{	
	delete m_block_top;
	m_block_top = block;
}

void Square::set_block_bot(Block* block)
{
	delete m_block_bot;
	m_block_bot = block;
}

Square::~Square()
{
	delete m_block_top;
	delete m_block_bot;
}

void Square::draw(sf::RenderTexture& chunk_texture)
{	
	if (!m_drawable)
		return;
	// Bot
	sf::Sprite sprite_bot;
	sprite_bot.setPosition(m_pixel_pos);
	m_block_bot->draw(sprite_bot);
	// Top
	sf::Sprite sprite_top;
	sprite_top.setPosition(m_pixel_pos);
	if (m_block_top->get_properties().solid)
	{
		if (!m_block_top->is_isolated())
			sprite_top.setColor(sf::Color(170, 170, 170));
		else
			sprite_top.setColor(sf::Color::Black);
	}
	m_block_top->draw(sprite_top);
	// Draw
	chunk_texture.draw(sprite_bot);
	chunk_texture.draw(sprite_top);
	// Borders
	if (!m_block_top->get_properties().solid)
	{
		sf::Sprite sprite_border;
		sprite_border.setPosition(m_pixel_pos);
		sprite_border.setTexture(*m_border_textures[m_block_top->get_solid_connexions()]);
		chunk_texture.draw(sprite_border);
	}
	// Highlight
	if (m_highlighted)
	{
		sf::RectangleShape hl_overlay;
		hl_overlay.setFillColor(sf::Color(255, 255, 255, 100));
		hl_overlay.setSize(sf::Vector2f(SPRITE_SIZE, SPRITE_SIZE));
		hl_overlay.setPosition(m_pixel_pos);
		chunk_texture.draw(hl_overlay);
	}
}

sf::Vector2i Square::get_pos(void) const
{
	return m_pos;
}

sf::Vector2i Square::get_chunk_pos(void) const
{
	return m_chunk_pos;
}

bool Square::is_drawable(void) const
{
	return m_drawable;
}

Block* Square::get_block_top(void) const
{
	return m_block_top;
}

Block* Square::get_block_bot(void) const
{
	return m_block_bot;
}
