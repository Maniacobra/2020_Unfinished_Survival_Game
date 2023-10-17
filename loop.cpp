#include "loop.h"

int gameLoop(Displayer& displayer, GameMap& game_map, InterfaceManager* game_interface)
{	
	BlockType selected_type = BLOCK_AIR;
	sf::Vector2f mouse_pos;
	Square* selected_square(0);
	Square* tmp_selected_square(0);
	Utils::TimeCounter edit_delay = Utils::TimeCounter(200);
	Player* player = nullptr;
	while (displayer.running())
	{	
		// Player
		player = game_map.get_player();
		game_map.update_player();
		displayer.set_cam_pos(player->get_square_pos());
		// Selection
		tmp_selected_square = game_map.get_square(displayer.get_map_mouse_pos());
		if (game_interface->is_mouse_in_ui())
		{
			if (selected_square != nullptr)
			{
				selected_square->unhighlight();
				displayer.draw_square(selected_square);
			}
		}
		else if (tmp_selected_square != nullptr && selected_square != tmp_selected_square)
		{	
			if (selected_square != nullptr)
			{
				selected_square->unhighlight();
				displayer.draw_square(selected_square);
			}
			selected_square = tmp_selected_square;
			selected_square->highlight();
			displayer.draw_square(selected_square);
			edit_delay.reset();
		}

		// EVENTS

		// Click
		if (displayer.mouse_click(sf::Mouse::Left))
		{
			if (!game_interface->click(false))
				game_map.hit_entity(displayer.get_map_mouse_pos(), 20);
		}
		else if (displayer.mouse_click(sf::Mouse::Right))
		{
			if (!game_interface->click(true))
				game_map.hit_entity(displayer.get_map_mouse_pos(), 20);
		}
		if (selected_square != nullptr && !game_interface->is_mouse_in_ui())
		{	
			// Right click
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{	
				if ((!edit_delay.started() || edit_delay.finished()) && game_interface->get_selected_material() != BLOCK_VOID)
				{
					bool count_pick = false;
					if (selected_square->get_block_top()->get_id() != game_interface->get_selected_material())
						count_pick = true;
					ItemStack drops = game_map.place_block(selected_square, game_interface->get_selected_material(), true, true);
					if (drops.get_item() == ITEM_NOTHING && drops.get_number() == 1)
						count_pick = false;
					else
						player->give_items(drops);
					if (count_pick)
						game_interface->pick_block();
					displayer.draw_square(selected_square);
				}
				edit_delay.start();
			}
			// Left click
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{	
				ItemStack drops = game_map.place_block(selected_square, BLOCK_AIR, true, true);
				player->give_items(drops);
				displayer.draw_square(selected_square);
			}
			// Rotate
			if (displayer.key_pressed(sf::Keyboard::F))
			{
				game_map.rot_block(selected_square, false);
				displayer.draw_square(selected_square);

			}
			else if (displayer.key_pressed(sf::Keyboard::G))
			{
				game_map.rot_block(selected_square, true);
				displayer.draw_square(selected_square);
			}
		}
		// Hotbar
		if (displayer.key_pressed(sf::Keyboard::Num1))
			game_interface->change_selection(0);
		else if (displayer.key_pressed(sf::Keyboard::Num2))
			game_interface->change_selection(1);
		else if (displayer.key_pressed(sf::Keyboard::Num3))
			game_interface->change_selection(2);
		else if (displayer.key_pressed(sf::Keyboard::Num4))
			game_interface->change_selection(3);
		else if (displayer.key_pressed(sf::Keyboard::Num5))
			game_interface->change_selection(4);
		else if (displayer.key_pressed(sf::Keyboard::Num6))
			game_interface->change_selection(5);
		else if (displayer.key_pressed(sf::Keyboard::Num7))
			game_interface->change_selection(6);
		else if (displayer.key_pressed(sf::Keyboard::Num8))
			game_interface->change_selection(7);
		else if (displayer.key_pressed(sf::Keyboard::Num9))
			game_interface->change_selection(8);
		// Inventory / Crafting
		if (displayer.key_pressed(sf::Keyboard::E))
			game_interface->toggle_inv_menu();
		// Scroll
		if (displayer.mouse_scrolled(true))
			game_interface->scroll(false);
		else if (displayer.mouse_scrolled(false))
			game_interface->scroll(true);
		// Restart
		if (displayer.key_pressed(sf::Keyboard::R))
		{
			game_map.restart();
			selected_square = nullptr;
			tmp_selected_square = nullptr;
		}
		// Draw + Update
		game_map.update();
		if (!displayer.update())
			return FCT_QUIT;
	}
	return FCT_QUIT;
}
