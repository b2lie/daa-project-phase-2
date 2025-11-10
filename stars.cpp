#include "stars.h"
#include "raylib.h"
#include <cstdlib>

std::vector<Star> stars;

void initStars(int count) {
    stars.clear();
    for (int i = 0; i < count; i++) {
        stars.push_back({ (float)GetRandomValue(0,800), (float)GetRandomValue(0,600), (float)GetRandomValue(1,3) });
    }
}

void updateAndDrawStars() {
    for (auto &s : stars) {
        s.y += s.speed * 0.3f;
        if (s.y > 600) s.y = 0;
        DrawPixel(s.x, s.y, LIGHTGRAY);
    }
}