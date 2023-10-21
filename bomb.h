#ifndef BOMB_H
#define BOMB_H


#include "game_types.h"

class bomb
{
public:
    position cur_position;

    unsigned short animation_timer;
    unsigned short lifetime;
    bool exploded;

    bomb();

    void draw(sf::RenderWindow &window);
    bool pacman_collision(const position &pacman_position);
};

#endif // BOMB_H
