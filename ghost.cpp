#include "ghost.h"

#include <array>
#include <cmath>
#include <vector>
#include "SFML/Graphics.hpp"

#include "game_types.h"
#include "pacman.h"
#include "tools.h"

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
    byte body_frame = (byte)(floor(animation_timer / (float)(GHOST_ANIMATION_SPEED)));

    sf::Sprite body;
    sf::Sprite face;

    sf::Texture texture;
    texture.loadFromFile("res/img/ghost" + std::to_string(CELL_SIZE) + ".png");

    body.setTexture(texture);
    body.setPosition(cur_position.x, cur_position.y);
    body.setTextureRect(sf::IntRect(CELL_SIZE * body_frame, 0, CELL_SIZE, CELL_SIZE));

    face.setTexture(texture);
    face.setPosition(cur_position.x, cur_position.y);

    if (!frightened)
    {
        switch (id)
        {
        case ghosts::red:
            body.setColor(sf::Color(255, 0, 0));
            break;
        case ghosts::pink:
            body.setColor(sf::Color(255, 182, 255));
            break;
        case ghosts::cyan:
            body.setColor(sf::Color(0, 255, 255));
            break;
        case ghosts::orange:
            body.setColor(sf::Color(255, 182, 85));
        }

        face.setTextureRect(sf::IntRect(CELL_SIZE * direction, CELL_SIZE, CELL_SIZE, CELL_SIZE));

        window.draw(body);
    }
    else if (frightened == frightened_modes::frightened)
    {
        body.setColor(sf::Color(36, 36, 255));
        face.setTextureRect(sf::IntRect(4 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));

        if (flash && body_frame % 2 == 0)
        {
            body.setColor(sf::Color(255, 255, 255));
            face.setColor(sf::Color(255, 0, 0));
        }
        else
        {
            body.setColor(sf::Color(36, 36, 255));
            face.setColor(sf::Color(255, 255, 255));
        }

        window.draw(body);
    }
    else
    {
        face.setTextureRect(sf::IntRect(CELL_SIZE * direction, 2 * CELL_SIZE, CELL_SIZE, CELL_SIZE));
    }

    window.draw(face);

    animation_timer = (1 + animation_timer) % (GHOST_ANIMATION_FRAMES * GHOST_ANIMATION_SPEED);
}

void ghost::reset(const position &home, const position &home_exit)
{
    movement_mode = 0;
    use_door = 0 < id;

    direction = 0;
    frightened = 0;
    frightened_speed_timer = 0;

    animation_timer = 0;

    this->home = home;
    this->home_exit = home_exit;
    target = home_exit;
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
    bool move = 0;
    byte available_ways = 0;
    byte speed = GHOST_SPEED;

    if (frightened == frightened_modes::not_frightened)
    {
        bomb_drop_timer++;

        if (bomb_drop_timer > BOMB_DROP_INTERVAL)
        {
            bomb b;
            b.cur_position = cur_position;
            bombs.push_back(b);
            bomb_drop_timer = 0;
        }
    }

    std::array<bool, 4> walls{};

    if (frightened == frightened_modes::not_frightened && pacman.energizer_timer == ENERGIZER_DURATION / pow(2, level))
    {
        frightened_speed_timer = GHOST_FRIGHTENED_SPEED;

        frightened = frightened_modes::frightened;
    }
    else if (!pacman.energizer_timer && frightened == frightened_modes::frightened)
    {
        frightened = frightened_modes::not_frightened;
    }

    if (frightened == frightened_modes::very_frightened && (cur_position.x % GHOST_ESCAPE_SPEED) == 0 && (cur_position.y % GHOST_ESCAPE_SPEED) == 0)
        speed = GHOST_ESCAPE_SPEED;

    update_target(pacman.direction, ghost_0.cur_position, pacman.cur_position);

    walls[0] = map_collision(use_door, {speed + cur_position.x, cur_position.y}, map);
    walls[1] = map_collision(use_door, {cur_position.x, cur_position.y - speed}, map);
    walls[2] = map_collision(use_door, {cur_position.x - speed, cur_position.y}, map);
    walls[3] = map_collision(use_door, {cur_position.x, speed + cur_position.y}, map);

    if (frightened != frightened_modes::frightened)
    {
        byte optimal_direction = directions::no_direction;

        move = 1;

        for (byte a = 0; a < 4; a++)
        {
            if (a == flip_direction(direction))
                continue;

            if (!walls[a])
            {
                if (optimal_direction == directions::no_direction)
                    optimal_direction = a;

                available_ways++;

                if (get_target_distance(a) < get_target_distance(optimal_direction))
                    optimal_direction = a;
            }
        }

        if (available_ways > 1)
            direction = optimal_direction;
        else
        {
            if (optimal_direction == directions::no_direction)
                direction = flip_direction(direction);
            else
                direction = optimal_direction;
        }
    }
    else
    {
        byte random_direction = rand() % 4;

        if (!frightened_speed_timer)
        {
            move = 1;

            frightened_speed_timer = GHOST_FRIGHTENED_SPEED;

            for (byte a = 0; a < 4; a++)
            {
                if (a == flip_direction(direction))
                    continue;

                if (!walls[a])
                    available_ways++;
            }

            if (0 < available_ways)
            {
                while (walls[random_direction] || random_direction == flip_direction(direction))
                    random_direction = rand() % 4;
                direction = random_direction;
            }
            else
                direction = flip_direction(direction);
        }
        else
            frightened_speed_timer--;
    }

    if (move)
    {
        switch (direction)
        {
        case directions::right:
            cur_position.x += speed;
            break;
        case directions::up:
            cur_position.y -= speed;
            break;
        case directions::left:
            cur_position.x -= speed;
            break;
        case directions::down:
            cur_position.y += speed;
        }
        if (-CELL_SIZE >= cur_position.x)
            cur_position.x = CELL_SIZE * MAP_WIDTH - speed;
        else if (cur_position.x >= CELL_SIZE * MAP_WIDTH)
            cur_position.x = speed - CELL_SIZE;
    }

    if (pacman_collision(pacman.cur_position))
    {
        if (!frightened)
        {
            pacman.is_dead = true;
        }
        else
        {
            use_door = true;
            frightened = frightened_modes::very_frightened;
            target = home;
            pacman.score.add(POINTS_FOR_GHOST, level);
        }
    }
}

void ghost::update_target(byte pacman_direction, const position &ghost_0_position, const position &pacman_position)
{
    if (use_door)
    {
        if (cur_position == target)
        {
            if (home_exit == target)
            {
                use_door = 0;
            }
            else if (home == target)
            {
                frightened = frightened_modes::not_frightened;
                target = home_exit;
            }
        }
    }
    else
    {
        if (!movement_mode)
        {
            switch (id)
            {
            case ghosts::red:
                target = {CELL_SIZE * (MAP_WIDTH - 1), 0};
                break;
            case ghosts::pink:
                target = {0, 0};
                break;
            case ghosts::cyan:
                target = {CELL_SIZE * (MAP_WIDTH - 1), CELL_SIZE * (MAP_HEIGHT - 1)};
                break;
            case ghosts::orange:
                target = {0, CELL_SIZE * (MAP_HEIGHT - 1)};
            }
        }
        else
        {
            switch (id)
            {
            case ghosts::red:
            {
                target = pacman_position;

                break;
            }
            case ghosts::pink:
            {
                target = pacman_position;

                switch (pacman_direction)
                {
                case directions::right:
                    target.x += CELL_SIZE * GHOST_1_CHASE;
                    break;
                case directions::up:
                    target.y -= CELL_SIZE * GHOST_1_CHASE;
                    break;
                case directions::left:
                    target.x -= CELL_SIZE * GHOST_1_CHASE;
                    break;
                case directions::down:
                    target.y += CELL_SIZE * GHOST_1_CHASE;
                }

                break;
            }
            case ghosts::cyan:
            {
                target = pacman_position;
                switch (pacman_direction)
                {
                case 0:
                    target.x += CELL_SIZE * GHOST_2_CHASE;
                    break;
                case 1:
                    target.y -= CELL_SIZE * GHOST_2_CHASE;
                    break;
                case 2:
                    target.x -= CELL_SIZE * GHOST_2_CHASE;
                    break;
                case 3:
                    target.y += CELL_SIZE * GHOST_2_CHASE;
                }
                target.x += target.x - ghost_0_position.x;
                target.y += target.y - ghost_0_position.y;
                break;
            }
            case ghosts::orange:
            {
                if (CELL_SIZE * GHOST_3_CHASE <= sqrt(pow(cur_position.x - pacman_position.x, 2) + pow(cur_position.y - pacman_position.y, 2)))
                    target = pacman_position;
                else
                    target = {0, CELL_SIZE * (MAP_HEIGHT - 1)};
            }
            }
        }
    }
}
