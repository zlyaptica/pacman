#include "ghost_manager.h"

#include <math.h>
#include "SFML/Graphics.hpp"

#include "pacman.h"
#include "ghost_manager.h"

ghost_manager::ghost_manager() : current_wave(0),
                                 wave_timer(LONG_SCATTER_DURATION),
                                 ghosts({ghost(0), ghost(1), ghost(2), ghost(3)})
{
}

void ghost_manager::draw(bool flash, sf::RenderWindow &window)
{
    for (ghost &ghost : ghosts)
        ghost.draw(flash, window);
}

void ghost_manager::reset(byte level, const std::array<position, 4> &ghost_positions)
{
    current_wave = 0;

    wave_timer = (unsigned short)(LONG_SCATTER_DURATION / pow(2, level));

    for (byte a = 0; a < 4; a++)
        ghosts[a].cur_position = {ghost_positions[a].x, ghost_positions[a].y};

    for (ghost &ghost : ghosts)
        ghost.reset(ghosts[2].cur_position, ghosts[0].cur_position);
}

void ghost_manager::update(byte level, game_map &map, pacman &pacman, std::vector<bomb> &bombs)
{
    if (!pacman.energizer_timer)
    {
        if (!wave_timer)
        {
            if (7 > current_wave)
            {
                current_wave++;

                for (ghost &ghost : ghosts)
                    ghost.switch_mode();
            }

            if (1 == current_wave % 2)
                wave_timer = CHASE_DURATION;
            else if (2 == current_wave)
                wave_timer = (unsigned short)(LONG_SCATTER_DURATION / pow(2, level));
            else
                wave_timer = (unsigned short)(SHORT_SCATTER_DURATION / pow(2, level));
        }
        else
            wave_timer--;
    }

    for (ghost &ghost : ghosts)
        ghost.update(level, map, ghosts[0], pacman, bombs);
}

