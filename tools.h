#ifndef TOOLS_H
#define TOOLS_H



#include "game_types.h"
#include "pacman.h"
#include "score.h"

game_map make_game_map(const std::array<std::string, MAP_HEIGHT> &map_from_strings, std::array<position, 4> &ghost_positions, pacman &pacman);

void free_map(game_map p);

bool map_collision(bool use_door, position point, game_map& map);

bool collect_pellets(position point, game_map &map, score &score, int level);

#endif // TOOLS_H

