#include "tools.h"

#include <math.h>
#include <array>
#include <string>
#include "SFML/Graphics.hpp"

game_map make_game_map(const std::array<std::string, MAP_HEIGHT> &map_sketch, std::array<position, 4> &ghost_positions, pacman &pacman)
{
    game_map output_map;

    for (byte a = 0; a < MAP_HEIGHT; a++)
    {
        for (byte b = 0; b < MAP_WIDTH; b++)
        {
            output_map[b][a] = cell::empty;

            switch (map_sketch[a][b])
            {
            case '#':
                output_map[b][a] = cell::wall;
                break;
            case '=':
                output_map[b][a] = cell::door;
                break;
            case '.':
                output_map[b][a] = cell::pellet;
                break;
            case '0':
                ghost_positions[0].x = CELL_SIZE * b;
                ghost_positions[0].y = CELL_SIZE * a;
                break;
            case '1':
                ghost_positions[1].x = CELL_SIZE * b;
                ghost_positions[1].y = CELL_SIZE * a;
                break;
            case '2':
                ghost_positions[2].x = CELL_SIZE * b;
                ghost_positions[2].y = CELL_SIZE * a;
                break;
            case '3':
                ghost_positions[3].x = CELL_SIZE * b;
                ghost_positions[3].y = CELL_SIZE * a;
                break;
            case 'P':
                pacman.cur_position = {CELL_SIZE * b, CELL_SIZE * a};
                break;
            case 'o':
                output_map[b][a] = cell::energizer;
            }
        }
    }

    return output_map;
}

std::array<position, 4> calculate_map_coordinates(position point, game_map &map)
{
    std::array<position, 4> output;
    float cell_x = point.x / (float)(CELL_SIZE);
    float cell_y = point.y / (float)(CELL_SIZE);

    output[0] = {(int)floor(cell_x), (int)floor(cell_y)};
    output[1] = {(int)ceil(cell_x), (int)floor(cell_y)};
    output[2] = {(int)floor(cell_x), (int)ceil(cell_y)};
    output[3] = {(int)ceil(cell_x), (int)ceil(cell_y)};

    return output;
}

bool map_collision(bool use_door, position point, game_map &map)
{
    bool output = 0;

    std::array<position, 4> corners = calculate_map_coordinates(point, map);

    for (position &corner : corners)
    {
        int x = corner.x;
        int y = corner.y;

        if (0 <= x && 0 <= y && MAP_HEIGHT > y && MAP_WIDTH > x)
        {
            if (map[x][y] == cell::wall)
                output = 1;
            else if (!use_door && map[x][y] == cell::door)
                output = 1;
        }
    }

    return output;
}

bool collect_pellets(position point, game_map &map, score &score, int level)
{
    bool output = 0;

    std::array<position, 4> corners = calculate_map_coordinates(point, map);

    for (position &corner : corners)
    {
        int x = corner.x;
        int y = corner.y;

        if (map[x][y] == cell::energizer)
        {
            output = 1;
            map[x][y] = cell::empty;
        }
        else if (map[x][y] == cell::pellet)
        {
            map[x][y] = cell::empty;
            score.add(POINTS_FOR_PELLET, level);
        }
    }
    return output;
}

