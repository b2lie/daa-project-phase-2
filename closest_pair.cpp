#include "closest_pair.h"
#include "stars.h"
#include <fstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <thread> // for sleep
using namespace std;

vector<Point> readPointsFromFile(const string &filename) {
    vector<Point> points;
    ifstream fin(filename);
    double x, y;
    while (fin >> x >> y)
        points.push_back({x, y});
    return points;
}

// helpers :D
bool compareX(const Point &a, const Point &b) { return a.x < b.x; }
bool compareY(const Point &a, const Point &b) { return a.y < b.y; }
double dist(const Point &a, const Point &b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

static void sleepMs(int ms) { this_thread::sleep_for(chrono::milliseconds(ms)); }

// brute forcing for small sets
pair<Point, Point> bruteForce(const vector<Point> &pts, int l, int r) {
    double minD = 1e9;
    pair<Point, Point> res;
    for (int i = l; i <= r; i++)
        for (int j = i + 1; j <= r; j++)
        {
            double d = dist(pts[i], pts[j]);
            if (d < minD)
                res = {pts[i], pts[j]}, minD = d;
        }
    return res;
}

// recursive divide & conquer ++ animation
pair<Point, Point> closestPairRec(vector<Point> &Px, vector<Point> &Py, int l, int r, vector<Point> &allPoints) {
    // draw current subset and midline
    BeginDrawing();
    ClearBackground(BLACK);

    // draw all points
    for (auto &p : allPoints)
        DrawCircle(p.x, p.y, 4, WHITE);

    // highlight current subset
    for (int i = l; i <= r; i++)
        DrawCircle(Px[i].x, Px[i].y, 5, PURPLE);

    // midline
    int mid = (l + r) / 2;
    DrawLine(Px[mid].x, 0, Px[mid].x, 600, GRAY);

    EndDrawing();
    sleepMs(200);

    if (r - l <= 3)
        return bruteForce(Px, l, r);

    // divide
    Point midPoint = Px[mid];
    vector<Point> PyLeft, PyRight;
    for (auto &p : Py)
    {
        if (p.x <= midPoint.x)
            PyLeft.push_back(p);
        else
            PyRight.push_back(p);
    }

    auto dl = closestPairRec(Px, PyLeft, l, mid, allPoints);
    auto dr = closestPairRec(Px, PyRight, mid + 1, r, allPoints);
    double d = min(dist(dl.first, dl.second), dist(dr.first, dr.second));
    pair<Point, Point> minPair = (dist(dl.first, dl.second) < dist(dr.first, dr.second)) ? dl : dr;

    // strip
    vector<Point> strip;
    for (auto &p : Py)
        if (fabs(p.x - midPoint.x) < d)
            strip.push_back(p);

    for (size_t i = 0; i < strip.size(); i++) {
        for (size_t j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < d; j++) {
            double ds = dist(strip[i], strip[j]);
            if (ds < d) {
                d = ds;
                minPair = {strip[i], strip[j]};

                // drawing temporary candidate pairs
                BeginDrawing();
                for (auto &p : allPoints)
                    DrawCircle(p.x, p.y, 4, WHITE);
                DrawCircle(minPair.first.x, minPair.first.y, 6, RED);
                DrawCircle(minPair.second.x, minPair.second.y, 6, RED);
                DrawLine(minPair.first.x, minPair.first.y, minPair.second.x, minPair.second.y, PURPLE);
                EndDrawing();
                sleepMs(200);
            }
        }
        return minPair;
    }
}

pair<Point, Point> closestPairDivideAndConquer(vector<Point> pts) {
    vector<Point> Px = pts, Py = pts;
    sort(Px.begin(), Px.end(), compareX);
    sort(Py.begin(), Py.end(), compareY);
    return closestPairRec(Px, Py, 0, pts.size() - 1, pts);
}

// visualization function
void visualizeClosestPair(const string &filename) {
    vector<Point> pts = readPointsFromFile(filename);
    auto result = closestPairDivideAndConquer(pts);

    // final display: highlighting closest pair in pink
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) 
            break;

        BeginDrawing();
        ClearBackground(BLACK);

        updateAndDrawStars();

        for (auto &p : pts)
            DrawCircle(p.x, p.y, 4, WHITE);

        // final closest pair
        DrawCircle(result.first.x, result.first.y, 6, PINK);
        DrawCircle(result.second.x, result.second.y, 6, PINK);
        DrawLine(result.first.x, result.first.y, result.second.x, result.second.y, PURPLE);

        // displaying coordinates
        char buf[50];
        sprintf(buf, "Point 1: (%.1f, %.1f)", result.first.x, result.first.y);
        DrawText(buf, 20, 535, 20, WHITE);

        sprintf(buf, "Point 2: (%.1f, %.1f)", result.second.x, result.second.y);
        DrawText(buf, 20, 560, 20, WHITE);

        Rectangle headerBg = {855, 540, 660, 30}; // x, y, width, height
        DrawRectangleRec(headerBg, BLACK);      // black box
        DrawText("Press ESC to return", 900, 560, 18, GRAY);

        EndDrawing();
    }
}