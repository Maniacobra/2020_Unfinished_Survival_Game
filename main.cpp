#include "loop.h"

int main(void)
{	
	// Display
	Displayer displayer(WIN_X, WIN_Y);
	// Res
	struct ProgramData {
		TextureResources textures;
	} program_res;
	// Map
	GameMap game_map(program_res.textures, &displayer);
	// Interface
	InterfaceManager game_interface(&program_res.textures, game_map.get_player()->get_inventory());
	displayer.set_interface(&game_interface);
	// Loop
	gameLoop(displayer, game_map, &game_interface);
	return 0;
}