#include "map.h"

SquareMap::SquareMap()
{
	m_textures = nullptr;
}

SquareMap::SquareMap(TextureResources& textures)
{
	m_textures = &textures;
}

void SquareMap::init_new_square(sf::Vector2i pos, Square* new_square)
{
	m_grid[pos] = new_square;
	m_all_squares.push_back(new_square);
}

void SquareMap::tick_update(void)
{
	for (auto it(m_ticked_blocks.begin()); it != m_ticked_blocks.end(); ++it)
		(*it)->update();
	for (auto it(m_ticked_blocks.begin()); it != m_ticked_blocks.end(); ++it)
		(*it)->post_update();
}

ItemStack SquareMap::place_block(Square* square, BlockType id, bool top, bool placed_by_player, BlockOrientation rot, std::vector<float> params)
{
	if (square == nullptr || (placed_by_player && square->get_block_top()->is_isolated()))
		return ItemStack();
	if (placed_by_player)
		switch (square->get_block_top()->get_solid_connexions())
			case 254: case 251: case 239: case 191:
				return ItemStack(ITEM_NOTHING, 1);
	if (top && square->get_block_top()->get_id() == id && square->get_block_top()->get_rot() == rot)
	{
		square->get_block_top()->edit(params);
		return ItemStack(ITEM_NOTHING, 2);
	}
	// Items
	ItemStack drops;
	if (top && placed_by_player)
		drops = ItemStack(square->get_block_top()->get_properties().drop, square->get_block_top()->get_properties().drop_count);
	// Verif
	sf::Vector2i square_pos(square->get_pos());
	// Neighbors
	Square* neighbors[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	Block* square_neighbors[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	if (top)
	{
		neighbors[0] = get_square(square_pos.x - 1, square_pos.y - 1);
		if (neighbors[0] != nullptr)
			square_neighbors[0] = neighbors[0]->get_block_top();
		neighbors[1] = get_square(square_pos.x, square_pos.y - 1);
		if (neighbors[1] != nullptr)
			square_neighbors[1] = neighbors[1]->get_block_top();
		neighbors[2] = get_square(square_pos.x + 1, square_pos.y - 1);
		if (neighbors[2] != nullptr)
			square_neighbors[2] = neighbors[2]->get_block_top();
		neighbors[3] = get_square(square_pos.x + 1, square_pos.y);
		if (neighbors[3] != nullptr)
			square_neighbors[3] = neighbors[3]->get_block_top();
		neighbors[4] = get_square(square_pos.x + 1, square_pos.y + 1);
		if (neighbors[4] != nullptr)
			square_neighbors[4] = neighbors[4]->get_block_top();
		neighbors[5] = get_square(square_pos.x, square_pos.y + 1);
		if (neighbors[5] != nullptr)
			square_neighbors[5] = neighbors[5]->get_block_top();
		neighbors[6] = get_square(square_pos.x - 1, square_pos.y + 1);
		if (neighbors[6] != nullptr)
			square_neighbors[6] = neighbors[6]->get_block_top();
		neighbors[7] = get_square(square_pos.x - 1, square_pos.y);
		if (neighbors[7] != nullptr)
			square_neighbors[7] = neighbors[7]->get_block_top();
	}
	else
	{
		neighbors[0] = get_square(square_pos.x - 1, square_pos.y - 1);
		if (neighbors[0] != nullptr)
			square_neighbors[0] = neighbors[0]->get_block_bot();
		neighbors[1] = get_square(square_pos.x, square_pos.y - 1);
		if (neighbors[1] != nullptr)
			square_neighbors[1] = neighbors[1]->get_block_bot();
		neighbors[2] = get_square(square_pos.x + 1, square_pos.y - 1);
		if (neighbors[2] != nullptr)
			square_neighbors[2] = neighbors[2]->get_block_bot();
		neighbors[3] = get_square(square_pos.x + 1, square_pos.y);
		if (neighbors[3] != nullptr)
			square_neighbors[3] = neighbors[3]->get_block_bot();
		neighbors[4] = get_square(square_pos.x + 1, square_pos.y + 1);
		if (neighbors[4] != nullptr)
			square_neighbors[4] = neighbors[4]->get_block_bot();
		neighbors[5] = get_square(square_pos.x, square_pos.y + 1);
		if (neighbors[5] != nullptr)
			square_neighbors[5] = neighbors[5]->get_block_bot();
		neighbors[6] = get_square(square_pos.x - 1, square_pos.y + 1);
		if (neighbors[6] != nullptr)
			square_neighbors[6] = neighbors[6]->get_block_bot();
		neighbors[7] = get_square(square_pos.x - 1, square_pos.y);
		if (neighbors[7] != nullptr)
			square_neighbors[7] = neighbors[7]->get_block_bot();
	}
	// New block
	Block* new_block;
	if (id == BLOCK_GRASS)
		new_block = new Grass;
	else
		new_block = new Block;
	// Destroy and set
	if (top)
	{
		if (square->get_block_top()->is_active() && !square->get_block_bot()->is_active())
			m_active_squares.remove(square);
		if (square->get_block_top()->is_ticked())
			m_ticked_blocks.remove(square->get_block_top());
		square->get_block_top()->destroy();
		square->set_block_top(new_block);
	}
	else
	{
		if (square->get_block_bot()->is_active() && !square->get_block_top()->is_active())
			m_active_squares.remove(square);
		if (square->get_block_bot()->is_ticked())
			m_ticked_blocks.remove(square->get_block_bot());
		square->get_block_bot()->destroy();
		square->set_block_bot(new_block);
	}
	new_block->init(id, *m_textures, square_neighbors, rot, placed_by_player, params);
	// Push
	if (new_block->is_active())
		m_active_squares.push_back(square);
	if (new_block->is_ticked())
		m_ticked_blocks.push_back(new_block);
	// Covering
	if (top && ((square->get_block_bot()->is_covered() && id == BLOCK_AIR) || (!square->get_block_bot()->is_covered() && id != BLOCK_AIR)))
		square->get_block_bot()->switch_cover();
	else if (!top && square->get_block_top()->get_id() != BLOCK_AIR)
		new_block->switch_cover();
	// Redraw neighbors
	for (int i = 0; i < 8; i++)
	{	
		if (neighbors[i] != nullptr)
			m_toDraw.insert(neighbors[i]);
	}
	return drops;
}

void SquareMap::rot_block(Square* square, bool clockwise)
{
	if (square == nullptr)
		return;
	if (square->get_block_top()->get_id() != BLOCK_AIR)
		place_block(square, square->get_block_top()->get_id(), true, true,
			Utils::rotate(clockwise, square->get_block_top()->get_rot()), square->get_block_top()->get_param());
	else
		place_block(square, square->get_block_bot()->get_id(), true, true,
			Utils::rotate(clockwise, square->get_block_bot()->get_rot()), square->get_block_bot()->get_param());
}

std::set<Square*> SquareMap::get_toDraw(void)
{	
	std::set<Square*> return_list(m_toDraw);
	m_toDraw.clear();
	return return_list;
}

Square* SquareMap::get_square(sf::Vector2f pos)
{
	double x = pos.x / (double)SPRITE_SIZE;
	double y = pos.y / (double)SPRITE_SIZE;
	x += MAP_MAX_SIZE / 2;
	y += MAP_MAX_SIZE / 2;
	return m_grid[sf::Vector2i((int)x, (int)y)];
}

Square* SquareMap::get_square(int x, int y)
{	
	if (m_grid.count(sf::Vector2i(x, y)))
		return m_grid[sf::Vector2i(x, y)];
	return nullptr;
}

void SquareMap::restart()
{
	for (auto it(m_all_squares.begin()); it != m_all_squares.end(); ++it)
		delete (*it);
	m_all_squares.clear();
	m_grid.clear();
	m_ticked_blocks.clear();
	m_active_squares.clear();
}


////////////////////////////////////////////////////////////////////////////////////////////// GAMEMAP


GameMap::GameMap(TextureResources& textures, Displayer* displayer)
{	
	m_displayer = displayer;
	m_textures = &textures;
	sf::Vector2i m_center_pos = sf::Vector2i(0, 0);
	// Tick
	m_tick_counter = Utils::TimeCounter(TICK_DURATION);
	m_tick_counter.start();
	// Square map
	m_world_gen = WorldGenerator(0);
	m_square_map = SquareMap(*m_textures);
	// Create world
	std::vector<Square*> grass_blocks_list;
	std::vector<Chunk*> to_load = m_displayer->prepare_chunk_loading(m_center_pos, true);
	for (auto it(to_load.begin()); it != to_load.end(); ++it)
	{
		for (int i = 0; i < CHUNKS_SIZE * CHUNKS_SIZE; i++)
		{
			Square* new_square = init_square(*it);
			if (new_square != nullptr)
				grass_blocks_list.push_back(new_square);
		}
	}
	// Create player
	sf::Vector2i spawn_pos;
	if (grass_blocks_list.size() == 0)
	{
		logw("\nUnable to find grass blocks !");
		spawn_pos = sf::Vector2i(MAP_MAX_SIZE / 2, MAP_MAX_SIZE / 2);
	}
	else
	{
		Square* random_block = grass_blocks_list[rand() % grass_blocks_list.size()];
		spawn_pos = random_block->get_pos();
	}
	m_player = new Player(&m_square_map, m_textures->get_group("player"), spawn_pos);
	m_entities.insert(m_player);
	m_displayer->add_entity(m_player);
	logw("\nPlayer created at square pos :");
	logw(spawn_pos);
}

Square* GameMap::init_square(Chunk* chunk)
{
	int x = (chunk->get_nb_squares()) % CHUNKS_SIZE + chunk->get_pos().x;
	int y = (chunk->get_nb_squares()) / CHUNKS_SIZE + chunk->get_pos().y;
	bool is_grass = false;
	if (x >= 0 && y >= 0 && x < MAP_MAX_SIZE && y < MAP_MAX_SIZE && chunk->get_nb_squares() < CHUNKS_SIZE * CHUNKS_SIZE)
	{
		Square* new_square = new Square(sf::Vector2i(x, y), m_textures->get_group("border"));
		m_square_map.init_new_square(sf::Vector2i(x, y), new_square);
		if (!chunk->add_square(new_square))
		{
			logw("Error while placing square : ");
			logw(new_square->get_pos());
			logw('\n');
		}
		GenResult gen_result = m_world_gen.generate(x, y);
		BlockOrientation rand_direction = static_cast<BlockOrientation>(rand() % 4);
		std::vector<float> params;
		switch (gen_result.square)
		{
		case GRASS:
			m_square_map.place_block(new_square, BLOCK_AIR, true);
			params.push_back((float)gen_result.value);
			m_square_map.place_block(new_square, BLOCK_GRASS, false, false, TOP, params);
			is_grass = true;
			break;
		case TREE:
			params.push_back((float)gen_result.value);
			m_square_map.place_block(new_square, BLOCK_AIR, true);
			m_square_map.place_block(new_square, BLOCK_GRASS, false, false, TOP, params);
			plant_tree(sf::Vector2i(x, y));
			break;
		case GRASS_AND_STICKS:
			params.push_back((float)gen_result.value);
			m_square_map.place_block(new_square, BLOCK_STICKS, true, false, rand_direction);
			m_square_map.place_block(new_square, BLOCK_GRASS, false, false, TOP, params);
			break;
		case SAND:
			m_square_map.place_block(new_square, BLOCK_AIR, true);
			m_square_map.place_block(new_square, BLOCK_SAND, false);
			break;
		case MOUNTAIN:
			m_square_map.place_block(new_square, BLOCK_STONE);
			m_square_map.place_block(new_square, BLOCK_STONE, false);
			break;
		case STONE:
			m_square_map.place_block(new_square, BLOCK_AIR);
			m_square_map.place_block(new_square, BLOCK_STONE, false);
			break;
		case WATER:
			m_square_map.place_block(new_square, BLOCK_AIR);
			m_square_map.place_block(new_square, BLOCK_WATER, false);
			break;
		}
		if (chunk->get_nb_squares() == CHUNKS_SIZE * CHUNKS_SIZE)
			chunk->display();
		if (is_grass)
			return new_square;
	}
	return nullptr;
}

void GameMap::update(void)
{	
	// Ticked updates
	if (m_tick_counter.finished())
	{	
		m_tick_counter.start();
		// Loading chunks
		std::vector<Chunk*> to_load = m_displayer->prepare_chunk_loading(m_center_pos, false);
		for (auto it(to_load.begin()); it != to_load.end(); ++it)
			for (int i = 0; i < BLOCK_PER_FRAME; i++)
				init_square(*it);
		// Entities
		std::vector<Entity*> dead_list;
		for (auto it(m_entities.begin()); it != m_entities.end(); ++it)
		{
			(*it)->update();
			if ((*it)->is_dead())
				dead_list.push_back(*it);
		}
		// Delete dead entities
		for (auto it(dead_list.begin()); it != dead_list.end(); ++it)
		{	
			m_entities.erase(*it);
			m_displayer->remove_entity(*it);
			delete* it;
		}
	}
	// ToUpdate
	std::set<Square*> toDraw = m_square_map.get_toDraw();
	for (auto it(toDraw.begin()); it != toDraw.end(); ++it)
		m_displayer->draw_square(*it);
}

bool GameMap::hit_entity(sf::Vector2f mouse_pos, int dmg)
{
	for (auto it(m_entities.begin()); it != m_entities.end(); ++it)
	{
		if ((*it)->is_mouseOn(mouse_pos))
		{
			(*it)->hit(m_player, dmg);
			if ((*it)->is_dead())
			{
				m_player->get_inventory()->add_items((*it)->get_loot());
			}
			return true;
		}
	}
	return false;
}

ItemStack GameMap::place_block(Square* square, BlockType id, bool top, bool placed_by_player, BlockOrientation rot, std::vector<float> params)
{
	return m_square_map.place_block(square, id, top, placed_by_player, rot, params);
}

bool GameMap::hit_block(Square* square, bool top)
{
	return false;
}

void GameMap::rot_block(Square* square, bool clockwise)
{
	m_square_map.rot_block(square, clockwise);
}

Square* GameMap::get_square(sf::Vector2f pos)
{
	return m_square_map.get_square(pos);
}

void GameMap::update_player(void)
{	
	m_player->nonticked_update();
}

Player* GameMap::get_player(void)
{
	return m_player;
}

void GameMap::plant_tree(sf::Vector2i pos)
{
	Tree* tree = new Tree(&m_square_map, m_textures->get_group("tree"), pos);
	m_entities.insert(tree);
	m_displayer->add_entity(tree);
}

void GameMap::restart()
{
	m_square_map.restart();
	for (auto it(m_entities.begin()); it != m_entities.end(); ++it)
		if (*it != m_player)
			delete (*it);
	m_entities.clear();
	m_entities.insert(m_player);
	m_displayer->clear();
	m_displayer->add_entity(m_player);
	// Recreate world
	std::vector<Chunk*> to_load = m_displayer->prepare_chunk_loading(m_center_pos, true);
	for (auto it(to_load.begin()); it != to_load.end(); ++it)
		for (int i = 0; i < CHUNKS_SIZE * CHUNKS_SIZE; i++)
			init_square(*it);
}
