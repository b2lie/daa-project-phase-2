#include "raylib.h"
#include "stars.h"
#include "closest_pair.h"
#include "int_mult.h"
#include <fstream>
#include <iostream>

struct Button
{
    Rectangle bounds;
    const char *label;
    Color color;
};

// helper to check mouse click inside a button
bool IsButtonClicked(Button btn)
{
    return CheckCollisionPointRec(GetMousePosition(), btn.bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

int main()
{
    InitWindow(800, 600, "Divide & Conquer Visualizer");
    SetTargetFPS(60);
    initStars(150);

    Button buttons[3] = {
        {{280, 220, 250, 50}, "Closest Pair", LIGHTGRAY},
        {{280, 290, 250, 50}, "Integer Multiplication", LIGHTGRAY},
        {{280, 360, 250, 50}, "Exit", LIGHTGRAY}};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        updateAndDrawStars(); // stars move in bg!

        DrawText("Divide & Conquer Visualizer", 190, 120, 30, PURPLE);
        DrawText("DAA Project, Sem 5 - Fall 2025", 230, 160, 20, LIGHTGRAY);

        // draw and handle buttons
        for (int i = 0; i < 3; i++)
        {
            // hover effect
            if (CheckCollisionPointRec(GetMousePosition(), buttons[i].bounds))
                buttons[i].color = PINK;
            else
                buttons[i].color = LIGHTGRAY;

            DrawRectangleRec(buttons[i].bounds, buttons[i].color);
            DrawRectangleLinesEx(buttons[i].bounds, 2, PINK);

            int fontSize = 20;
            int textWidth = MeasureText(buttons[i].label, fontSize);
            int textX = buttons[i].bounds.x + (buttons[i].bounds.width - textWidth) / 2;
            int textY = buttons[i].bounds.y + (buttons[i].bounds.height - fontSize) / 2;

            DrawText(buttons[i].label, textX, textY, fontSize, BLACK);
        }

        // handle button clicks
        if (IsButtonClicked(buttons[0]))
        {
            // flash blank screen for 10 frames
            for (int i = 0; i < 10; i++)
            {
                BeginDrawing();
                ClearBackground(BLACK); // blank screen
                updateAndDrawStars();   // optional: keep stars moving
                EndDrawing();
            }

            // now run the full visualization
            visualizeClosestPair("input/closest100.txt");
        }
        else if (IsButtonClicked(buttons[1]))
        {
            for (int i = 0; i < 10; i++)
            {
                BeginDrawing();
                ClearBackground(BLACK);
                updateAndDrawStars();
                EndDrawing();
            }

            std::ifstream fin("input/karatsubaMult.txt");
            if (!fin.is_open())
            {
                std::cerr << "Error: could not open input/karatsubaMult.txt\n";
                return 1;
            }
            std::string xStr, yStr;
            if (!(fin >> xStr >> yStr)) {
                std::cerr << "Error reading numbers\n";
                return 1;
            }
            else
            {
                std::cout << "Read values: " << xStr << " and " << yStr << "\n";
            }

            visualizeKaratsuba(xStr, yStr);
        }
        else if (IsButtonClicked(buttons[2]))
        {
            break; // exit button
        }

        int fontSize = 18;

        // group title
        const char *title = "Group Members:";
        int titleWidth = MeasureText(title, fontSize);
        DrawText(title, (GetScreenWidth() - titleWidth) / 2, 450, fontSize, PINK);

        // member names
        const char *members[] = {"Batool Kazmi - 23K-0672", "Zunaira Ali 23K-0751", "Muhammad Zohib 23K-0602"};
        for (int i = 0; i < 3; i++)
        {
            int nameWidth = MeasureText(members[i], fontSize);
            DrawText(members[i], (GetScreenWidth() - nameWidth) / 2, 490 + (i * 20), fontSize, GRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}