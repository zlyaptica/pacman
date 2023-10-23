#include "graphics.h"

#include "SFML/Graphics.hpp"
#include <math.h>

void draw_map(game_map &map, sf::RenderWindow &window)
{
    sf::Sprite sprite;

    sf::Texture texture;
    texture.loadFromFile("img/map" + std::to_string(CELL_SIZE) + ".png");

    sprite.setTexture(texture);

    for (int a = 0; a < MAP_WIDTH; a++)
    {
        for (int b = 0; b < MAP_HEIGHT; b++)
        {
            sprite.setPosition((float)(CELL_SIZE * a), (float)(CELL_SIZE * b));
            switch (map[a][b])
            {
            case cell::empty:
                break;
            case cell::door:
                sprite.setTextureRect(sf::IntRect(2 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));
                window.draw(sprite);
                break;
            case cell::energizer:
                sprite.setTextureRect(sf::IntRect(CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));
                window.draw(sprite);
                break;
            case cell::pellet:
                sprite.setTextureRect(sf::IntRect(0, CELL_SIZE, CELL_SIZE, CELL_SIZE));
                window.draw(sprite);
                break;
            case cell::wall:
                bool down = 0;
                bool left = 0;
                bool right = 0;
                bool up = 0;

                if (b < MAP_HEIGHT - 1)
                    if (map[a][1 + b] == cell::wall)
                        down = 1;

                if (a > 0)
                {
                    if (map[a - 1][b] == cell::wall)
                        left = 1;
                }
                else
                    left = 1;

                if (a < MAP_WIDTH - 1)
                {
                    if (map[1 + a][b] == cell::wall)
                        right = 1;
                }
                else
                    right = 1;

                if (b > 0)
                    if (map[a][b - 1] == cell::wall)
                        up = 1;

                sprite.setTextureRect(sf::IntRect(CELL_SIZE * (down + 2 * (left + 2 * (right + 2 * up))), 0, CELL_SIZE, CELL_SIZE));

                window.draw(sprite);
            }
        }
    }
}

void draw_text(bool center, position pos, std::string text, sf::RenderWindow &window)
{
    short character_x = pos.x;
    short character_y = pos.y;

    byte character_width;

    sf::Sprite character_sprite;
    sf::Texture font_texture;

    font_texture.loadFromFile("img/font.png");

    character_width = font_texture.getSize().x / 96;
    character_sprite.setTexture(font_texture);

    if (center)
    {
        character_x = (short)(round(0.5f * (CELL_SIZE * MAP_WIDTH - character_width * text.substr(0, text.find_first_of('\n')).size())));
        character_y = (short)(round(0.5f * (CELL_SIZE * MAP_HEIGHT - FONT_HEIGHT * (1 + std::count(text.begin(), text.end(), '\n')))));
    }

    for (std::string::const_iterator a = text.begin(); a != text.end(); a++)
    {
        if ('\n' == *a)
        {
            if (center)
                character_x = (short)(round(0.5f * (CELL_SIZE * MAP_WIDTH - character_width * text.substr(1 + a - text.begin(), text.find_first_of('\n', 1 + a - text.begin()) - (1 + a - text.begin())).size())));
            else
                character_x = pos.x;
            character_y += FONT_HEIGHT;
            continue;
        }
        character_sprite.setPosition(character_x, character_y);
        character_sprite.setTextureRect(sf::IntRect(character_width * (*a - 32), 0, character_width, FONT_HEIGHT));
        character_x += character_width;
        window.draw(character_sprite);
    }
}

