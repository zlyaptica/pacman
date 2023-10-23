#include <array>
#include <vector>
#include <chrono>
#include <ctime>
#include "SFML/Graphics.hpp"

#include "game_types.h"
#include "ghost_manager.h"
#include "tools.h"
#include "graphics.h"

int main()
{
    bool game_won = 0;

    unsigned lag = 0;

    byte level = 0;

    std::chrono::time_point<std::chrono::steady_clock> previous_time;

    std::array<std::string, MAP_HEIGHT> map_sketch = {
        " ################### ",
        " #........#........# ",
        " #o##.###.#.###.##o# ",
        " #.................# ",
        " #.##.#.#####.#.##.# ",
        " #....#...#...#....# ",
        " ####.### # ###.#### ",
        "    #.#   0   #.#    ",
        "#####.# ##=## #.#####",
        "     .  #123#  .     ",
        "#####.# ##### #.#####",
        "    #.#       #.#    ",
        " ####.# ##### #.#### ",
        " #........#........# ",
        " #.##.###.#.###.##.# ",
        " #o.#.....P.....#.o# ",
        " ##.#.#.#####.#.#.## ",
        " #....#...#...#....# ",
        " #.######.#.######.# ",
        " #.................# ",
        " ################### "};

    game_map map;

    std::array<position, 4> ghost_positions;

    std::vector<bomb> bombs;

    sf::Event event;

    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) * SCREEN_RESIZE), "Pac-Man", sf::Style::Close);

    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));

    ghost_manager ghost_manager;

    pacman pacman;

    srand(time(0));

    map = make_game_map(map_sketch, ghost_positions, pacman);

    ghost_manager.reset(level, ghost_positions);

    previous_time = std::chrono::steady_clock::now();

    pacman.score.load();

    while (window.isOpen())
    {
        unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();

        lag += delta_time;

        previous_time += std::chrono::microseconds(delta_time);

        while (FRAME_DURATION <= lag)
        {
            lag -= FRAME_DURATION;

            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                case sf::Event::Closed:
                    pacman.score.save();
                    window.close();
                default:
                    break;
                }
            }

            for (long unsigned int i = 0; i < bombs.size(); i++)
                if (bombs[i].exploded)
                    bombs.erase(bombs.begin() + i);

            for (bomb &b : bombs)
                if (b.pacman_collision(pacman.cur_position))
                    pacman.is_dead = true;

            if (!game_won && !pacman.is_dead)
            {
                game_won = 1;
                pacman.update(level, map);
                ghost_manager.update(level, map, pacman, bombs);

                for (int i = 0; i < MAP_WIDTH; i++)
                {
                    for (int j = 0; j < MAP_HEIGHT; j++)
                        if (map[i][j] == cell::pellet)
                        {
                            game_won = 0;
                            break;
                        }

                    if (!game_won)
                        break;
                }

                if (game_won)
                    pacman.animation_timer = 0;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                game_won = 0;
                if (pacman.is_dead)
                {
                    level = 0;
                    pacman.score.points = 0;
                    pacman.score.save();
                }
                else
                    level++;

                map = make_game_map(map_sketch, ghost_positions, pacman);

                ghost_manager.reset(level, ghost_positions);

                pacman.reset();

                bombs.clear();
            }

            if (FRAME_DURATION > lag)
            {
                window.clear();

                if (!game_won && !pacman.is_dead)
                {
                    draw_map(map, window);

                    for (bomb &b : bombs)
                        b.draw(window);

                    ghost_manager.draw(GHOST_FLASH_START >= pacman.energizer_timer, window);

                    draw_text(0, {0, CELL_SIZE * MAP_HEIGHT}, "Level: " + std::to_string(1 + level), window);
                    draw_text(0, {CELL_SIZE * MAP_WIDTH - CELL_SIZE * 16, CELL_SIZE * MAP_HEIGHT}, "Score: " + std::to_string(pacman.score.points), window);
                    draw_text(0, {CELL_SIZE * MAP_WIDTH - CELL_SIZE * 8, CELL_SIZE * MAP_HEIGHT}, "Record: " + std::to_string(pacman.score.points_record), window);
                }

                pacman.draw(game_won, window);

                if (pacman.is_animation_over)
                {
                    if (game_won)
                        draw_text(1, {0, 0}, "Next level!", window);
                    else
                        draw_text(1, {0, 0}, "Game over!", window);
                }

                window.display();
            }
        }
    }
}
