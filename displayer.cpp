#include "displayer.h"

Displayer::Displayer(int win_x, int win_y)
{
	m_window.create(sf::VideoMode(win_x, win_y), "Unnamed Survival Game");
	m_view = m_window.getDefaultView();
	m_last_second = (long)time(NULL);
	m_fps = 0;
	m_nb_chunks_loading = 0;
	m_game_interface = nullptr;
	m_updates_count = 0;
	m_events.mouse_left = false;
	m_events.mouse_right = false;
	m_events.mouse_middle = false;
	m_events.scroll_up = false;
	m_events.scroll_down = false;
}

void Displayer::set_interface(InterfaceManager* game_interface)
{
	m_game_interface = game_interface;
}

void Displayer::set_cam_pos(sf::Vector2<double> pos)
{	
	m_cam_pos = sf::Vector2f((float)(pos.x - MAP_MAX_SIZE / 2), (float)(pos.y - MAP_MAX_SIZE / 2));
	m_cam_pos *= (float)SPRITE_SIZE;
	m_view.setCenter(m_cam_pos);
	m_window.setView(m_view);
}

bool Displayer::update(void)
{	
	// FPS
	m_updates_count++;
	if (time(NULL) - m_last_second > 0)
	{
		m_fps = m_updates_count;
		m_updates_count = 0;
		m_last_second = (long)time(NULL);
	}
	// Reset events
	m_events.keys.clear();
	m_events.mouse_left = false;
	m_events.mouse_middle = false;
	m_events.mouse_right = false;
	m_events.scroll_up = false;
	m_events.scroll_down = false;
	// Events
	sf::Event win_event;
	while (m_window.pollEvent(win_event))
	{
		switch (win_event.type)
		{
		case sf::Event::Closed:
			return false;
		case sf::Event::MouseWheelScrolled:
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			{	
				if (win_event.mouseWheelScroll.delta > 0)
					m_events.scroll_up = true;
				else
					m_events.scroll_down = true;
			}
			else
				zoom((int)win_event.mouseWheelScroll.delta);
			break;
		case sf::Event::Resized:
			resize();
			break;
		case sf::Event::MouseButtonPressed:
			switch (win_event.mouseButton.button)
			{
			case (sf::Mouse::Left):
				m_events.mouse_left = true;
				break;
			case (sf::Mouse::Middle):
				m_events.mouse_middle = true;
				break;
			case (sf::Mouse::Right):
				m_events.mouse_right = true;
				break;
			}
			break;
		case sf::Event::KeyPressed:
			m_events.keys.push_back(win_event.key.code);
			break;
		}
	}
	// BG
	m_window.clear(sf::Color(0, 0, 0));
	// Draw chunks
	sf::FloatRect draw_zone(m_view.getCenter() - m_view.getSize() / 2.f * CUSTOM_DRAW_ZONE_RATIO, m_view.getSize() * CUSTOM_DRAW_ZONE_RATIO);
	for (auto it(m_all_chunks.begin()); it != m_all_chunks.end(); ++it)
	{
		if ((*it)->is_inWindow(draw_zone) && (*it)->get_nb_squares() == CHUNKS_SIZE * CHUNKS_SIZE)
			(*it)->draw(m_window);
	}
	// Entities
	for (auto it(m_bot_entities.begin()); it != m_bot_entities.end(); ++it)
		if ((*it)->is_inWindow(draw_zone))
			(*it)->draw(m_window);
	for (auto it(m_top_entities.begin()); it != m_top_entities.end(); ++it)
		if ((*it)->is_inWindow(draw_zone))
			(*it)->draw(m_window);
	for (auto it(m_bot_entities.begin()); it != m_bot_entities.end(); ++it)
		if ((*it)->is_inWindow(draw_zone))
			(*it)->draw_hp_bar(m_window);
	for (auto it(m_top_entities.begin()); it != m_top_entities.end(); ++it)
		if ((*it)->is_inWindow(draw_zone))
			(*it)->draw_hp_bar(m_window);
	// Interface
	m_game_interface->update_and_draw(m_window, get_win_mouse_pos(), get_map_mouse_pos(), m_fps, m_nb_chunks_loading);
	m_window.setView(m_view);
	// Visible Zone
	if (CUSTOM_DRAW_ZONE_RATIO < 1)
	{
		sf::RectangleShape visible_zone;
		visible_zone.setSize(sf::Vector2f(draw_zone.width, draw_zone.height));
		visible_zone.setPosition(sf::Vector2f(draw_zone.left, draw_zone.top));
		visible_zone.setFillColor(sf::Color(0, 0, 0, 0));
		visible_zone.setOutlineColor(sf::Color::Red);
		visible_zone.setOutlineThickness(5.f);
		m_window.draw(visible_zone);
	}
	m_window.display();
	return true;
}

void Displayer::zoom(int ticks)
{	
	if (m_view.getSize().x < MIN_ZOOM && ticks < 0 || m_view.getSize().x > MAX_ZOOM && ticks > 0)
		return;
	m_view.zoom(ticks < 0 ? pow(1 / ZOOM_RATIO, ticks) : pow(ZOOM_RATIO, -ticks));
	m_window.setView(m_view);
}

void Displayer::resize(void)
{
	m_view.setSize(sf::Vector2f(m_window.getSize()));
	m_window.setView(m_view);
}

void Displayer::add_chunk(sf::Vector2i pos)
{	
	Chunk* new_chunk(new Chunk(pos));
	m_chunks_grid[pos] = new_chunk;
	m_all_chunks.push_back(new_chunk);
}

std::vector<Chunk*> Displayer::prepare_chunk_loading(sf::Vector2i pos, bool large)
{
	std::vector<Chunk*> chunks_list = std::vector<Chunk*>();
	sf::Vector2f center_f = m_window.getView().getCenter();
	center_f.x = ((center_f.x / SPRITE_SIZE) + MAP_MAX_SIZE / 2) / CHUNKS_SIZE;
	center_f.y = ((center_f.y / SPRITE_SIZE) + MAP_MAX_SIZE / 2) / CHUNKS_SIZE;
	sf::Vector2i center = sf::Vector2i((int)(std::floor(center_f.x)), (int)(std::floor(center_f.y)));
	for (auto it(m_all_chunks.begin()); it != m_all_chunks.end(); ++it)
	{
		if ((*it)->get_nb_squares() < CHUNKS_SIZE * CHUNKS_SIZE)
			chunks_list.push_back(*it);
	}
	int distance = (int)(CHUNK_LOADING_DISTANCE * (large ? 1.5f : 1.f));
	for (int iy = center.y - distance; iy < center.y + distance; iy++)
	{
		for (int ix = center.x - distance; ix < center.x + distance; ix++)
		{	
			pos.x = ix;
			pos.y = iy;
			if (pos.x >= 0 && pos.y >= 0 && pos.x < MAP_MAX_SIZE / CHUNKS_SIZE && pos.y < MAP_MAX_SIZE / CHUNKS_SIZE)
			{	
				if (m_chunks_grid[pos] == nullptr)
				{
					add_chunk(pos);
					chunks_list.push_back(m_chunks_grid[pos]);
				}
			}
		}
	}
	m_nb_chunks_loading = chunks_list.size();
	return chunks_list;
}

bool Displayer::add_square(Square* square)
{
	for (auto it(m_chunks_grid.begin()); it != m_chunks_grid.end(); ++it)
	{
		if (it->second->add_square(square))
		{	
			it->second->draw_square(square);
			return true;
		}
	}
	return false;
}

void Displayer::draw_square(Square* square)
{	
	if (square->is_drawable())
		m_chunks_grid[square->get_chunk_pos()]->draw_square(square);
}

void Displayer::add_entity(Entity* entity)
{	
	if (entity->is_top_layer())
		m_top_entities.insert(entity);
	else
		m_bot_entities.insert(entity);
}

void Displayer::remove_entity(Entity* entity)
{
	if (entity->is_top_layer())
		m_top_entities.erase(entity);
	else
		m_bot_entities.erase(entity);
}

void Displayer::clear()
{	
	m_chunks_grid.clear();
	for (auto it(m_all_chunks.begin()); it != m_all_chunks.end(); ++it)
	{
		delete (*it);
	}
	m_all_chunks.clear();
	m_top_entities.clear();
	m_bot_entities.clear();
}

sf::Vector2f Displayer::get_win_mouse_pos(void) const
{
	sf::Vector2f pos(sf::Mouse::getPosition());
	pos -= (sf::Vector2f)m_window.getPosition();
	pos.x += MOUSE_X_CORRECTION;
	pos.y += MOUSE_Y_CORRECTION;
	pos.x = pos.x / m_window.getSize().x * WIN_X;
	pos.y = pos.y / m_window.getSize().y * WIN_Y;
	return pos;
}

sf::Vector2f Displayer::get_map_mouse_pos(void) const
{	
	sf::Vector2i pos(sf::Mouse::getPosition());
	pos -= m_window.getPosition();
	pos.x += MOUSE_X_CORRECTION;
	pos.y += MOUSE_Y_CORRECTION;
	if (pos.x < 0)
		pos.x = 0;
	else if (pos.x >= (signed int)m_window.getSize().x)
		pos.x = m_window.getSize().x - 1;
	if (pos.y < 0)
		pos.y = 0;
	else if (pos.y >= (signed int)m_window.getSize().y)
		pos.y = m_window.getSize().y - 1;
	sf::Vector2f map_pos(m_window.mapPixelToCoords(pos));
	return map_pos;
}

bool Displayer::running(void) const
{
	return m_window.isOpen();
}

bool Displayer::mouse_click(int button) const
{	
	switch (button)
	{
	case sf::Mouse::Left:
		return m_events.mouse_left;
	case sf::Mouse::Middle:
		return m_events.mouse_middle;
	case sf::Mouse::Right:
		return m_events.mouse_right;
	}
	return false;
}

bool Displayer::key_pressed(int key) const
{
	for (auto it(m_events.keys.begin()); it != m_events.keys.end(); ++it)
	{
		if (*it == key)
			return true;
	}
	return false;
}

bool Displayer::mouse_scrolled(bool upward) const
{
	if (upward && m_events.scroll_up)
		return true;
	if (!upward && m_events.scroll_down)
		return true;
	return false;
}

int Displayer::get_fps(void) const
{
	return m_fps;
}