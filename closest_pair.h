#ifndef CLOSEST_PAIR_H
#define CLOSEST_PAIR_H

#include <vector>
#include <string>
#include "raylib.h"

struct Point {
    double x, y;
};

std::vector<Point> readPointsFromFile(const std::string &filename);
std::pair<Point, Point> closestPairDivideAndConquer(std::vector<Point> pts);
void visualizeClosestPair(const std::string &filename);

#endif