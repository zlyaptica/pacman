#include "bomb.h"

#include <math.h>

bomb::bomb() : cur_position({0, 0}),
               animation_timer(0),
               lifetime(0),
               exploded(0)
{
}

void bomb::draw(sf::RenderWindow &window)
{
    byte frame = (byte)(floor(animation_timer / (float)(BOMB_ANIMATION_SPEED)));

    sf::Sprite sprite;

    sf::Texture texture;

    texture.loadFromFile("img/bomb" + std::to_string(CELL_SIZE) + ".png");

    sprite.setPosition(cur_position.x, cur_position.y);

    sprite.setTexture(texture);

    lifetime++;

    if (lifetime > BOMB_MAX_LIFETIME)
    {
        sprite.setTextureRect(sf::IntRect(CELL_SIZE * (BOMB_BOOM_FRAME - 1), 0, CELL_SIZE, CELL_SIZE));
        exploded = true;
    }
    else
    {
        sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, 0, CELL_SIZE, CELL_SIZE));
    }

    window.draw(sprite);

    animation_timer = (1 + animation_timer) % (BOMB_ANIMATION_FRAMES * BOMB_ANIMATION_SPEED);
}

bool bomb::pacman_collision(const position &pacman_position)
{
    if (cur_position.x > pacman_position.x - CELL_SIZE && cur_position.x < CELL_SIZE + pacman_position.x)
        if (cur_position.y > pacman_position.y - CELL_SIZE && cur_position.y < CELL_SIZE + pacman_position.y)
            return 1;

    return 0;
}

