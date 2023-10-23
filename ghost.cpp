#include "ghost.h"

ghost::ghost(byte id) : id(id)
{
}

bool ghost::pacman_collision(const position &pacman_position)
{
    if (cur_position.x > pacman_position.x - CELL_SIZE && cur_position.x < CELL_SIZE + pacman_position.x)
        if (cur_position.y > pacman_position.y - CELL_SIZE && cur_position.y < CELL_SIZE + pacman_position.y)
            return 1;

    return 0;
}

float ghost::get_target_distance(byte direction)
{
    short x = cur_position.x;
    short y = cur_position.y;

    switch (direction)
    {
    case 0:
        x += GHOST_SPEED;
        break;
    case 1:
        y -= GHOST_SPEED;
        break;
    case 2:
        x -= GHOST_SPEED;
        break;
    case 3:
        y += GHOST_SPEED;
    }

    return (float)(sqrt(pow(x - target.x, 2) + pow(y - target.y, 2)));
}

void ghost::draw(bool flash, sf::RenderWindow &window)
{

}

void ghost::reset(const position &home, const position &home_exit)
{

}

void ghost::switch_mode()
{
    movement_mode = 1 - movement_mode;
}

byte flip_direction(byte direction)
{
    return (2 + direction) % 4;
}

void ghost::update(byte level, game_map &map, ghost &ghost_0, pacman &pacman, std::vector<bomb> &bombs)
{

}

void ghost::update_target(byte pacman_direction, const position &ghost_0_position, const position &pacman_position)
{

}
