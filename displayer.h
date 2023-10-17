#pragma once
#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include <list>
#include <map>
#include <SFML/Graphics.hpp>
#include <vector>

#include "entity.h"
#include "interface.h"
#include "square.h"
#include "utils.h"

class Chunk
{	
	sf::Vector2i m_chunk_pos;
	sf::Sprite m_sprite;
	sf::RenderTexture m_texture;
	sf::IntRect m_rect;
	std::vector<Square*> m_squares;
	int m_drawed_squares;

public:

	Chunk(sf::Vector2i pos);
	// Methods
	bool add_square(Square* square);
	void display(void);
	void draw(sf::RenderWindow& window);
	void move(sf::Vector2f direction);
	void draw_square(Square* square);
	// Const
	bool is_inWindow(sf::FloatRect draw_zone) const;
	sf::Vector2i get_pos(void) const;
	sf::Vector2i get_chunk_pos(void) const;
	int get_nb_squares(void) const;
	std::vector<Square*> get_squares(void) const;
};

class Displayer
{	
	sf::RenderWindow m_window;
	InterfaceManager* m_game_interface;
	std::set<Entity*> m_top_entities;
	std::set<Entity*> m_bot_entities;
	std::list<Chunk*> m_all_chunks;
	std::map<sf::Vector2i, Chunk*, Utils::Vec2i_predicate> m_chunks_grid;
	sf::View m_view;
	sf::Vector2f m_cam_pos;
	struct Events
	{
		bool mouse_left = false;
		bool mouse_right = false;
		bool mouse_middle = false;
		std::vector<int> keys;
		bool scroll_up = false;
		bool scroll_down = false;
	} m_events;
	// Div
	long m_last_second;
	int m_updates_count;
	int m_fps;
	int m_nb_chunks_loading;
	// Methods
	void add_chunk(sf::Vector2i pos);

public:

	Displayer(int win_x=WIN_X, int win_y=WIN_Y);
	// Methods
	void set_interface(InterfaceManager* game_interface);
	void set_cam_pos(sf::Vector2<double> pos);
	bool update(void);
	void zoom(int ticks);
	void resize(void);
	std::vector<Chunk*> prepare_chunk_loading(sf::Vector2i pos, bool large);
	bool add_square(Square* square);
	void draw_square(Square* square);
	void add_entity(Entity* entity);
	void remove_entity(Entity* entity);
	void clear();
	// Const
	sf::Vector2f get_win_mouse_pos(void) const;
	sf::Vector2f get_map_mouse_pos(void) const;
	bool running(void) const;
	bool mouse_click(int button) const;
	bool key_pressed(int key) const;
	bool mouse_scrolled(bool upward) const;
	int get_fps(void) const;
};

#endif // !DISPLAY_H_INCLUDED