#ifndef PACMAN_H
#define PACMAN_H


#include "game_types.h"
#include "score.h"

class pacman
{
public:
    bool is_animation_over;

    bool is_dead;

    byte direction;

    unsigned short animation_timer;
    unsigned short energizer_timer;

    position cur_position;

    score score;

    pacman();

    void draw(bool victory, sf::RenderWindow &window);
    void reset();
    void update(byte level, game_map &map);
};

#endif // PACMAN_H
