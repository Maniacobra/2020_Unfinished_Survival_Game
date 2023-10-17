#pragma once
#ifndef LOOP_H_INCLUDED
#define LOOP_H_INCLUDED

#include "displayer.h"
#include "interface.h"
#include "map.h"
#include "utils.h"

int gameLoop(Displayer& displayer, GameMap& game_map, InterfaceManager* game_interface);

#endif // !LOOP_H_INCLUDED
