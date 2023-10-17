#pragma once
#ifndef ANIMATION_H_INCLUDED

#include "res.h"

class Animation
{
private:

	int m_frame_nb;
	bool m_mirror;
	TextureGroup m_textures;
	int m_tick_delay;
	bool m_paused;
	int m_count;

public:

	Animation(void)
	{
		m_mirror = false;
		m_textures = TextureGroup();
		m_tick_delay = 0;
		m_frame_nb = 0;
		m_paused = false;
		m_count = 0;
	}
	Animation(TextureGroup textures, bool mirror, int tick_delay)
	{
		m_mirror = mirror;
		m_textures = textures;
		m_tick_delay = tick_delay;
		m_frame_nb = 0;
		m_paused = false;
		m_count = 0;
	};
	void update(void)
	{	
		if (m_paused)
			return;
		m_count++;
		if (m_count >= m_tick_delay)
		{
			m_frame_nb++;
			if (m_frame_nb >= m_textures.size())
			{
				if (m_mirror)
					m_frame_nb = -m_textures.size() + 2;
				else
					m_frame_nb = 0;
			}
			m_count = 0;
		}
	}
	void stop(int frame)
	{
		m_paused = true;
		m_frame_nb = frame;
	}
	void run(int frame)
	{
		m_paused = false;
		m_frame_nb = frame;
		m_count = 0;
	}
	sf::Texture* get_texture(void)
	{
		return m_textures[abs(m_frame_nb)];
	}
	bool is_running(void)
	{
		return !m_paused;
	}
};

#endif // !ANIMATION_H_INCLUDED
