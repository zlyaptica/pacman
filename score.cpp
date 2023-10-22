#include "score.h"

#include <fstream>

void score::save()
{
    std::ofstream file("max_score.txt", std::ofstream::out);
    file << points_record;
    file.close();
}

void score::load()
{
    std::ifstream file("max_score.txt", std::ifstream::in);

    if (!file.is_open())
        return;
    file >> points_record;
    file.close();
}

void score::add(int points, int level)
{
    this->points += points * (level + 1);
    if (this->points > points_record)
        points_record = this->points;
}

