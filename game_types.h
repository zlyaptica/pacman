#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include "constants.h"
#include "SFML/Graphics.hpp"
#include <array>

typedef unsigned char byte;

enum cell
{
    empty,
    wall,
    door,
    pellet,
    energizer
};

enum directions
{
    right,
    up,
    left,
    down,
    no_direction
};

enum frightened_modes
{
    not_frightened,
    frightened,
    very_frightened
};

enum ghosts
{
    red,
    pink,
    cyan,
    orange
};

struct position
{
    int x;
    int y;
};

typedef std::array<std::array<cell, MAP_HEIGHT>, MAP_WIDTH> game_map;

bool operator==(position &a, position &b);

#endif // GAME_TYPES_H
