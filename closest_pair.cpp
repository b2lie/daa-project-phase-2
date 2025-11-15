#include "closest_pair.h"
#include "stars.h"
#include <fstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <thread> // for sleep

// --- read points from file ---
std::vector<Point> readPointsFromFile(const std::string &filename)
{
    std::vector<Point> points;
    std::ifstream fin(filename);
    double x, y;
    while (fin >> x >> y)
        points.push_back({x, y});
    return points;
}

// --- helpers ---
bool compareX(const Point &a, const Point &b) { return a.x < b.x; }
bool compareY(const Point &a, const Point &b) { return a.y < b.y; }
double dist(const Point &a, const Point &b)
{
    double dx = a.x - b.x, dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

static void sleepMs(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

// --- brute force for small sets ---
std::pair<Point, Point> bruteForce(const std::vector<Point> &pts, int l, int r)
{
    double minD = 1e9;
    std::pair<Point, Point> res;
    for (int i = l; i <= r; i++)
        for (int j = i + 1; j <= r; j++)
        {
            double d = dist(pts[i], pts[j]);
            if (d < minD)
                res = {pts[i], pts[j]}, minD = d;
        }
    return res;
}

// --- recursive divide & conquer with animation ---
std::pair<Point, Point> closestPairRec(std::vector<Point> &Px, std::vector<Point> &Py, int l, int r, std::vector<Point> &allPoints)
{
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
    std::vector<Point> PyLeft, PyRight;
    for (auto &p : Py)
    {
        if (p.x <= midPoint.x)
            PyLeft.push_back(p);
        else
            PyRight.push_back(p);
    }

    auto dl = closestPairRec(Px, PyLeft, l, mid, allPoints);
    auto dr = closestPairRec(Px, PyRight, mid + 1, r, allPoints);
    double d = std::min(dist(dl.first, dl.second), dist(dr.first, dr.second));
    std::pair<Point, Point> minPair = (dist(dl.first, dl.second) < dist(dr.first, dr.second)) ? dl : dr;

    // strip
    std::vector<Point> strip;
    for (auto &p : Py)
        if (fabs(p.x - midPoint.x) < d)
            strip.push_back(p);

    for (size_t i = 0; i < strip.size(); i++)
        for (size_t j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < d; j++)
        {
            double ds = dist(strip[i], strip[j]);
            if (ds < d)
            {
                d = ds;
                minPair = {strip[i], strip[j]};

                // draw temporary candidate pair
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

// --- main D&C function ---
std::pair<Point, Point> closestPairDivideAndConquer(std::vector<Point> pts)
{
    std::vector<Point> Px = pts, Py = pts;
    std::sort(Px.begin(), Px.end(), compareX);
    std::sort(Py.begin(), Py.end(), compareY);
    return closestPairRec(Px, Py, 0, pts.size() - 1, pts);
}

// --- visualization function ---
void visualizeClosestPair(const std::string &filename)
{
    std::vector<Point> pts = readPointsFromFile(filename);
    auto result = closestPairDivideAndConquer(pts);

    // final display: highlight closest pair in pink
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE)) 
            break;

        BeginDrawing();
        ClearBackground(BLACK);

        // stars background
        updateAndDrawStars();

        // all points
        for (auto &p : pts)
            DrawCircle(p.x, p.y, 4, WHITE);

        // final closest pair
        DrawCircle(result.first.x, result.first.y, 6, PINK);
        DrawCircle(result.second.x, result.second.y, 6, PINK);
        DrawLine(result.first.x, result.first.y, result.second.x, result.second.y, PURPLE);

        // display coordinates
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