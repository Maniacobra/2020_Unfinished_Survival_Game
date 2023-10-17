#pragma once
#ifndef RES_H_INCLUDED
#define RES_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "utils.h"

class TextureGroup
{
	std::vector<sf::Texture*> m_group;
	sf::Texture* m_default_texture;

public:

	TextureGroup(sf::Texture* default_texture, std::vector<sf::Texture>& group)
	{
		for (auto it(group.begin()); it != group.end(); ++it)
			m_group.push_back(&*it);
		m_default_texture = default_texture;
	}
	TextureGroup(sf::Texture* default_texture)
	{
		m_default_texture = default_texture;
	}
	TextureGroup(void)
	{
		m_default_texture = nullptr;
	}
	sf::Texture* operator[](unsigned char nb)
	{
		if (nb < m_group.size())
			return m_group[nb];
		return m_default_texture;
	}
	int size(void)
	{
		return m_group.size();
	}
};

class TextureResources
{
	std::map<std::string, std::vector<sf::Texture> > m_textures;
	std::map<std::string, sf::Font> m_fonts;
	sf::Texture m_default_texture;
	// Method
	void create_coded_textures(void);

public:

	TextureResources(void);
	sf::Texture& get_texture(std::string file_name, unsigned int nb);
	TextureGroup get_group(std::string file_name);
	sf::Font& get_font(std::string file_name);
};

const std::string TEXTURE_FILES(
	// BLOCKS
	"blocks/grass.1"
	"blocks/cobblestone.1"
	"blocks/logs.1"
	"blocks/sand.1"
	"blocks/sticks_on_ground.5"
	"blocks/stone.1"
	"blocks/water.1"
	// ITEMS
	"items/log.1"
	"items/rock.1"
	"items/stick.1"
	"items/pickaxe.1"
	// ENTITIES
	"entities/player.5"
	"entities/tree.1"
	// UI
	"ui/arrow.1"
	"ui/craft.2"
	"ui/inv_case.1"
	"ui/select_square.1"
	"ui/toolbar.1"
);

const std::string DEFAULT_FONT_FILE = "resources/fonts/consola.ttf";
const std::string TITLE_FONT_FILE = "resources/fonts/frostbite.ttf";

#endif // !RES_H_INCLUDED
