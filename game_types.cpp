#include "game_types.h"

bool operator==(position &a, position &b)
{
    return a.x == b.x && a.y == b.y;
}
