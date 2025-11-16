#ifndef CLOSEST_PAIR_H
#define CLOSEST_PAIR_H

#include <vector>
#include <string>
#include "raylib.h"
using namespace std;

struct Point {
    double x, y;
};

vector<Point> readPointsFromFile(const string &filename);
pair<Point, Point> closestPairDivideAndConquer(vector<Point> pts);
void visualizeClosestPair(const string &filename);

#endif