#include "pacman.h"
#include <array>
#include <cmath>
#include "SFML/Graphics.hpp"

#include "game_types.h"
#include "tools.h"

pacman::pacman() : is_animation_over(0),
                   is_dead(0),
                   direction(0),
                   energizer_timer(0),
                   cur_position({0, 0}),
                   score({0,0})
{
}

void pacman::draw(bool is_victory, sf::RenderWindow &window)
{
    byte frame = (byte)(floor(animation_timer / (float)(PACMAN_ANIMATION_SPEED)));

    sf::Sprite sprite;

    sf::Texture texture;

    sprite.setPosition(cur_position.x, cur_position.y);

    if (is_dead || is_victory)
    {
        if (animation_timer < PACMAN_DEATH_FRAMES * PACMAN_ANIMATION_SPEED)
        {
            animation_timer++;

            texture.loadFromFile("img/pacman_death" + std::to_string(CELL_SIZE) + ".png");

            sprite.setTexture(texture);
            sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, 0, CELL_SIZE, CELL_SIZE));

            window.draw(sprite);
        }
        else
        {
            is_animation_over = true;
        }
    }
    else
    {
        texture.loadFromFile("img/pacman" + std::to_string(CELL_SIZE) + ".png");

        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, CELL_SIZE * direction, CELL_SIZE, CELL_SIZE));

        window.draw(sprite);

        animation_timer = (1 + animation_timer) % (PACMAN_ANIMATION_FRAMES * PACMAN_ANIMATION_SPEED);
    }
}

void pacman::reset()
{
    is_animation_over = 0;
    is_dead = 0;

    direction = 0;

    animation_timer = 0;
    energizer_timer = 0;
}

void pacman::update(byte level, game_map &map)
{
    std::array<bool, 4> walls{};
    walls[0] = map_collision(0, {PACMAN_SPEED + cur_position.x, cur_position.y}, map);
    walls[1] = map_collision(0, {cur_position.x, cur_position.y - PACMAN_SPEED}, map);
    walls[2] = map_collision(0, {cur_position.x - PACMAN_SPEED, cur_position.y}, map);
    walls[3] = map_collision(0, {cur_position.x, PACMAN_SPEED + cur_position.y}, map);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        if (!walls[directions::right])
            direction = directions::right;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        if (!walls[directions::up])
            direction = directions::up;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        if (!walls[directions::left])
            direction = directions::left;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        if (!walls[directions::down])
            direction = directions::down;

    if (!walls[direction])
        switch (direction)
        {
        case directions::right:
            cur_position.x += PACMAN_SPEED;
            break;
        case directions::up:
            cur_position.y -= PACMAN_SPEED;
            break;
        case directions::left:
            cur_position.x -= PACMAN_SPEED;
            break;
        case directions::down:
            cur_position.y += PACMAN_SPEED;
        }

    if (-CELL_SIZE >= cur_position.x)
        cur_position.x = CELL_SIZE * MAP_WIDTH - PACMAN_SPEED;
    else if (CELL_SIZE * MAP_WIDTH <= cur_position.x)
        cur_position.x = PACMAN_SPEED - CELL_SIZE;

    if (collect_pellets(cur_position, map, score, level))
        energizer_timer = (unsigned short)(ENERGIZER_DURATION / pow(2, level));
    else
        energizer_timer = std::max(0, energizer_timer - 1);
}


