#ifndef GHOST_H
#define GHOST_H


#include "game_types.h"
#include "pacman.h"
#include "bomb.h"

class ghost
{
public:
    bool movement_mode;

    bool use_door;

    byte direction;
    byte frightened;
    byte frightened_speed_timer;
    byte id;

    unsigned short animation_timer;
    unsigned short bomb_drop_timer;

    position home;
    position home_exit;
    position cur_position;
    position target;

    ghost(byte id);

    bool pacman_collision(const position &pacman_position);

    float get_target_distance(byte direction);

    void draw(bool flash, sf::RenderWindow &window);
    void reset(const position &home, const position &home_exit);
    void switch_mode();
    void update(byte level, game_map &map, ghost &ghost_0, pacman &pacman, std::vector<bomb> &bombs);
    void update_target(byte pacman_direction, const position &ghost_0_position, const position &pacman_position);
};

#endif // GHOST_H
