#pragma once
#include <vector>
#include "raylib.h"
using namespace std;

struct Star {
    float x, y;
    float speed;
};

extern vector<Star> stars;

void initStars(int count);
void updateAndDrawStars();