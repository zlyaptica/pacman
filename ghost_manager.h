#ifndef GHOST_MANAGER_H
#define GHOST_MANAGER_H


#include "ghost.h"
#include <array>

class ghost_manager
{
    byte current_wave;
    unsigned short wave_timer;

    std::array<ghost, 4> ghosts;

public:
    ghost_manager();

    void draw(bool flash, sf::RenderWindow &window);
    void reset(byte level, const std::array<position, 4> &ghost_positions);
    void update(byte level, game_map &map, pacman &pacman, std::vector<bomb> &bombs);
};

#endif // GHOST_MANAGER_H
