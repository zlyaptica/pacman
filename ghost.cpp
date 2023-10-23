#include "ghost.h"

ghost::ghost(byte id) : id(id)
{
}

bool ghost::pacman_collision(const position &pacman_position)
{

}

float ghost::get_target_distance(byte direction)
{

}

void ghost::draw(bool flash, sf::RenderWindow &window)
{

}

void ghost::reset(const position &home, const position &home_exit)
{

}

void ghost::switch_mode()
{

}

byte flip_direction(byte direction)
{

}

void ghost::update(byte level, game_map &map, ghost &ghost_0, pacman &pacman, std::vector<bomb> &bombs)
{

}

void ghost::update_target(byte pacman_direction, const position &ghost_0_position, const position &pacman_position)
{

}
