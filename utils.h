#pragma once
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <chrono>
#include <SFML/System/Vector2.hpp>

#include "block_properties.h"
#include "log.h"
#include "program_const.h"

class Utils
{
public:

	static BlockOrientation swapside(int side)
	{
		return (BlockOrientation)(side < 4 ? side + 4 : side - 4);
	}

	static BlockOrientation rotate(bool clockwise, int rot)
	{	
		if (clockwise)
		{
			rot++;
			if (rot == 4)
				rot = 0;
		}
		else
		{
			if (rot == 0)
				rot = 3;
			else
				rot--;
		}
		return (BlockOrientation)rot;
	}

	static sf::Vector2f origin(BlockOrientation rot)
	{
		switch (rot)
		{
		case RIGHT:
			return sf::Vector2f(0, SPRITE_SIZE);
		case BOT:
			return sf::Vector2f(SPRITE_SIZE, SPRITE_SIZE);
		case LEFT:
			return sf::Vector2f(SPRITE_SIZE, 0);
		default:
			return sf::Vector2f(0, 0);
		}
	}

// CLASSES

	class Vec2i_predicate
	{
	public:
		bool operator()(sf::Vector2i const& v1, sf::Vector2i const& v2) const
		{
			if (v1.y < v2.y)
				return true;
			if (v1.y == v2.y && v1.x < v2.x)
				return true;
			return false;
		}
	};

	class TimeCounter
	{
		std::chrono::steady_clock::time_point m_start;
		unsigned long m_duration;
		bool m_started;

	public:

		TimeCounter(unsigned int duration_ms)
		{
			m_duration = duration_ms * 1000000;
			m_started = false;
		}
		
		TimeCounter()
		{
			m_duration = 1000000000l;
			m_started = false;
		}

		void start()
		{
			m_start = std::chrono::steady_clock::now();
			m_started = true;
		}

		double progress()
		{	
			if (m_started)
			{
				std::chrono::steady_clock::duration time = std::chrono::steady_clock::now() - m_start;
				double result = (long double)time.count() / (long double)m_duration;
				return result;
			}
			return 0;
		}

		void reset()
		{	
			m_started = false;
		}

		bool finished() const
		{
			if (m_started) {
				std::chrono::steady_clock::duration duration = std::chrono::steady_clock::now() - m_start;
				return duration.count() >= m_duration;
			}
			return false;
		}

		bool started() const
		{
			return m_started;
		}
	};
};

#endif // !UTILS_H_INCLUDED
