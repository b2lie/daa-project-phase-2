#pragma once
#include <vector>
#include "raylib.h"

struct Star {
    float x, y;
    float speed;
};

extern std::vector<Star> stars;

void initStars(int count);
void updateAndDrawStars();