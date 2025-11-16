#include "raylib.h"
#include "stars.h"
#include "closest_pair.h"
#include "int_mult.h"
#include <fstream>
#include <iostream>
#include <thread>
using namespace std;

struct Button {
    Rectangle bounds;
    const char *label;
    Color color;
};

// helper to check mouse click inside a button
bool IsButtonClicked(Button btn) {
    return CheckCollisionPointRec(GetMousePosition(), btn.bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

int main() {
    InitWindow(1120, 600, "Divide & Conquer Visualizer");

    Image icon = LoadImage("extras/icon.png");
    ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8); // <- force RGBA

    if (icon.data == NULL) {
        printf("Error: Could not load icon.png\n");
        CloseWindow();
        return 1;
    }
    SetWindowIcon(icon);   
    // we can unload the image after setting the icon!
    UnloadImage(icon);
    
    SetTargetFPS(60);
    initStars(150);

    Button buttons[3] = {
        {{425, 220, 250, 50}, "Closest Pair", LIGHTGRAY},
        {{425, 290, 250, 50}, "Integer Multiplication", LIGHTGRAY},
        {{425, 360, 250, 50}, "Exit", LIGHTGRAY}
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        updateAndDrawStars(); // stars move in bg!

        bool choiceMade = false;

        DrawText("Divide & Conquer Visualizer", 350, 100, 30, PURPLE);
        DrawText("DAA Project, Sem 5 - Fall 2025", 395, 150, 20, LIGHTGRAY);

        // drawing + handling buttons
        for (int i = 0; i < 3; i++) {
            // enabling hover effect
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

        // handling button interactions
        if (IsButtonClicked(buttons[0])){
            bool choiceMade = false;
            int selectedIndex = 0;
            const int numOptions = 10;
            const char *options[numOptions] = {
                "closest100.txt",
                "closest200.txt",
                "closest300.txt",
                "closest400.txt",
                "closest500.txt",
                "closest600.txt",
                "closest700.txt",
                "closest800.txt",
                "closest900.txt",
                "closest1000.txt"
            };

            while (!choiceMade && !WindowShouldClose()) {
                if (IsKeyPressed(KEY_DOWN)) {
                    selectedIndex++;
                    if (selectedIndex >= numOptions)
                        selectedIndex = 0; // wrap around
                }
                else if (IsKeyPressed(KEY_UP)) {
                    selectedIndex--;
                    if (selectedIndex < 0)
                        selectedIndex = numOptions - 1; // wrap around
                }
                else if (IsKeyPressed(KEY_ENTER)) {
                    choiceMade = true;
                }

                BeginDrawing();
                ClearBackground(BLACK);
                updateAndDrawStars();

                DrawText("Choose an input data set to visualize Closest Pair:", 300, 100, 20, LIGHTGRAY);

                for (int i = 0; i < numOptions; i++) {
                    int yPos = 150 + i * 30;

                    if (i == selectedIndex) {
                        // highlighting selected option
                        DrawRectangle(400, yPos - 4, 300, 28, PINK);
                        DrawText(options[i], 420, yPos, 18, RAYWHITE);
                    }
                    else {
                        DrawText(options[i], 420, yPos, 18, GRAY);
                    }
                }

                EndDrawing();
            }

            // running visualization with selected option
            string chosenFile = string("extras/pairs_inputs/") + options[selectedIndex];
            visualizeClosestPair(chosenFile.c_str());
        }
        else if (IsButtonClicked(buttons[1])) {
            bool choiceMade = false;
            int selectedIndex = 0;
            const int numOptions = 10;
            const char *options[numOptions] = {
                "karatsuba4.txt",
                "karatsuba10.txt",
                "karatsuba30.txt",
                "karatsuba40.txt",
                "karatsuba50.txt",
                "karatsuba60.txt",
                "karatsuba70.txt",
                "karatsuba80.txt",
                "karatsuba90.txt",
                "karatsuba100.txt"
            };

            while (!choiceMade && !WindowShouldClose()) {
                if (IsKeyPressed(KEY_DOWN)) {
                    selectedIndex++;
                    if (selectedIndex >= numOptions)
                        selectedIndex = 0; // wrap around
                }
                else if (IsKeyPressed(KEY_UP)) {
                    selectedIndex--;
                    if (selectedIndex < 0)
                        selectedIndex = numOptions - 1; // wrap around
                }
                else if (IsKeyPressed(KEY_ENTER)) {
                    choiceMade = true;
                }

                BeginDrawing();
                ClearBackground(BLACK);
                updateAndDrawStars();

                DrawText("Choose an input data set to visualize Karatsuba's Integer Multiplication:", 300, 100, 20, LIGHTGRAY);

                for (int i = 0; i < numOptions; i++) {
                    int yPos = 150 + i * 30;

                    if (i == selectedIndex) {
                        DrawRectangle(400, yPos - 4, 300, 28, PINK);
                        DrawText(options[i], 420, yPos, 18, RAYWHITE);
                    }
                    else {
                        DrawText(options[i], 420, yPos, 18, GRAY);
                    }
                }

                EndDrawing();
            }

            string chosenFile = string("extras/karatsubas_inputs/") + options[selectedIndex];
            ifstream fin(chosenFile);
            string xStr, yStr;
            if (!(fin >> xStr >> yStr))
            {
                cerr << "Error reading numbers\n";
                continue;
            }

            // launching background thread for computation
            logs.clear();
            string finalResult;

            thread calc([=, &finalResult]() mutable {
                finalResult = karatsubaRec(xStr, yStr);  // capturing final result
                logStep("RESULT: " + xStr + " × " + yStr + " = " + finalResult); });
            calc.detach();

            // visualization loop
            float scroll = 0;
            int logHeight = 45;    // match your line spacing in visualizeKaratsuba
            int headerOffset = 60; // offset from top for first log

            while (!WindowShouldClose()) {
                // manual scroll
                int totalHeight = logs.size() * logHeight + headerOffset;
                if (totalHeight > GetScreenHeight())
                    scroll -= GetMouseWheelMove() * 20; // scroll only if content exceeds window

                // clamp scroll
                int maxScroll = max(0, totalHeight - GetScreenHeight());
                if (scroll > maxScroll) scroll = maxScroll;
                if (scroll < 0) scroll = 0;

                BeginDrawing();
                ClearBackground(BLACK);

                updateAndDrawStars();
                visualizeKaratsuba(xStr, yStr, +scroll); // moves content upwards as it comes

                // drawing black background rectangle behind header
                Rectangle headerBg = {10, 20, 1120, 30}; // x, y, width, height
                DrawRectangleRec(headerBg, BLACK);       // black box
                DrawText(("Final Result: " + finalResult).c_str(), 10, 20, 20, GREEN);

                headerBg = {900, 560, 660, 30};
                DrawRectangleRec(headerBg, BLACK);
                DrawText("Press ESC to return", 900, 540, 18, GRAY);
                DrawText("Scroll down for steps", 900, 570, 18, GRAY);

                if (IsKeyPressed(KEY_ESCAPE))
                    break;

                EndDrawing();
            }
        }

        else if (IsButtonClicked(buttons[2])){
            break; // exiting!
        }

        int fontSize = 18;

        // credits
        const char *title = "Group Members:";
        int titleWidth = MeasureText(title, fontSize);
        DrawText(title, (GetScreenWidth() - titleWidth) / 2, 450, fontSize, PINK);

        const char *members[] = {"Batool Kazmi 23K-0672", "Zunaira Ali 23K-0751", "Muhammad Zohib 23K-0602"};
        for (int i = 0; i < 3; i++){
            int nameWidth = MeasureText(members[i], fontSize);
            DrawText(members[i], (GetScreenWidth() - nameWidth) / 2, 490 + (i * 20), fontSize, GRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}