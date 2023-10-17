#pragma once
#ifndef PROGRAM_CONST_H_INCLUDED
#define PROGRAM_CONST_H_INCLUDED

#include <string>

// Win
const int WIN_X = 700;
const int WIN_Y = 700;
// Events
const float ZOOM_RATIO = 0.95f;
const int MIN_ZOOM = 64;
const int MAX_ZOOM = 100000;
const int MOUSE_X_CORRECTION = -7;
const int MOUSE_Y_CORRECTION = -37;
const float CAM_SPEED = 4096.f;
// Generator
const int WORLD_STRETCHING_NB_PARAMETERS = 4;
const double WORLD_STRETCHING_VALUES[] = {210, 75, 30, 20};
const double WORLD_STRETCHING_EXTREMES[] = {7, 3, 0, 0};
// Map
const int CHUNKS_SIZE = 16;
const int SPRITE_SIZE = 16;
const float CUSTOM_DRAW_ZONE_RATIO = 1.f;
const int CHUNK_LOADING_DISTANCE = 4;
const long MAP_MAX_SIZE = 40000000l;
// Opti
const int BLOCK_PER_FRAME = 2;
// Gameplay
const int TICK_DURATION = 10;
// Player
const int PLAYER_SPEED = 5;
const int INVENTORY_SIZE = 39;
// Files
const std::string TEXTURE_FOLDER("resources/");
const std::string LOG_FILE("log.txt");
// Interface
const float TOOLBAR_SIZE = 3.f;
// Menus
enum { FCT_QUIT, LOOP_GAME };

#endif // !PROGRAM_CONST_H_INCLUDED
