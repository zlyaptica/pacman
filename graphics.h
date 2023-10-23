#ifndef GRAPHICS_H
#define GRAPHICS_H


#include "game_types.h"
#include "pacman.h"

void draw_map(game_map &map, sf::RenderWindow &window);
void draw_text(bool center, position pos, std::string text, sf::RenderWindow &window);



#endif // GRAPHICS_H
