#include "raylib.h"
#include <string>
#include <thread>
#include <iostream>
#include <algorithm>

static void sleepMs(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

// add leading zeros to make lengths equal
std::string padLeft(const std::string &s, int n) {
    return std::string(n - s.size(), '0') + s;
}

// string addition
std::string addStr(const std::string &a, const std::string &b) {
    int n = std::max(a.size(), b.size());
    std::string A = padLeft(a, n), B = padLeft(b, n);
    std::string res(n+1, '0');
    int carry = 0;
    for (int i = n-1; i >= 0; i--) {
        int sum = (A[i]-'0') + (B[i]-'0') + carry;
        res[i+1] = (sum%10) + '0';
        carry = sum/10;
    }
    if (carry) res[0] = carry+'0';
    return res[0]=='0' ? res.substr(1) : res;
}

// string subtraction (a >= b)
std::string subStr(const std::string &a, const std::string &b) {
    int n = a.size();
    std::string res(n, '0');
    int borrow = 0;
    for (int i = n-1; i >=0; i--) {
        int diff = (a[i]-'0') - (b[i]-'0') - borrow;
        if (diff<0) diff+=10, borrow=1;
        else borrow=0;
        res[i] = diff+'0';
    }
    size_t start = res.find_first_not_of('0');
    return start==std::string::npos ? "0" : res.substr(start);
}

// multiply small strings directly
std::string multStrSmall(const std::string &a, const std::string &b) {
    long long x = std::stoll(a);
    long long y = std::stoll(b);
    return std::to_string(x*y);
}

// recursive Karatsuba (strings) + visualization
std::string karatsubaRec(const std::string &x, const std::string &y, int depth, float xPos, float yPos, float offsetX) {
    BeginDrawing();
    DrawCircleLines(xPos, yPos, 30, WHITE);
    DrawText(("x=" + x).c_str(), xPos-50, yPos-20, 18, LIGHTGRAY);
    DrawText(("y=" + y).c_str(), xPos-50, yPos, 18, LIGHTGRAY);
    EndDrawing();
    sleepMs(400);

    if (x.size()<=4 || y.size()<=4) { // base case
        std::string res = multStrSmall(x,y);
        BeginDrawing();
        DrawText(res.c_str(), xPos-15, yPos+40, 18, YELLOW);
        EndDrawing();
        sleepMs(400);
        return res;
    }

    int n = std::max(x.size(), y.size());
    int m = n/2;

    std::string xPadded = padLeft(x,n);
    std::string yPadded = padLeft(y,n);

    std::string high1 = xPadded.substr(0,n-m);
    std::string low1  = xPadded.substr(n-m);
    std::string high2 = yPadded.substr(0,n-m);
    std::string low2  = yPadded.substr(n-m);

    float nextY = yPos + 100;
    float childOffset = offsetX / 2;

    BeginDrawing();
    DrawLine(xPos, yPos+30, xPos-childOffset, nextY-30, GRAY);
    DrawLine(xPos, yPos+30, xPos, nextY-30, GRAY);
    DrawLine(xPos, yPos+30, xPos+childOffset, nextY-30, GRAY);
    EndDrawing();

    std::string z2 = karatsubaRec(high1, high2, depth+1, xPos-childOffset, nextY, childOffset);
    std::string z1 = karatsubaRec(addStr(high1,low1), addStr(high2,low2), depth+1, xPos, nextY, childOffset);
    std::string z0 = karatsubaRec(low1, low2, depth+1, xPos+childOffset, nextY, childOffset);

    // compute final result: z2*10^(2*m) + (z1-z2-z0)*10^m + z0
    std::string z2shift = z2 + std::string(2*m,'0');
    std::string z1shift = subStr(subStr(z1,z2),z0) + std::string(m,'0');
    std::string res = addStr(addStr(z2shift,z1shift),z0);

    BeginDrawing();
    DrawText(res.c_str(), xPos-20, yPos+40, 18, YELLOW);
    EndDrawing();
    sleepMs(500);

    return res;
}

// main visualization
void visualizeKaratsuba(const std::string &x, const std::string &y) {
    InitWindow(1200,900,"Karatsuba String Multiplication Tree");
    SetTargetFPS(60);
    ClearBackground(BLACK);

    BeginDrawing();
    DrawText("Building recursion tree...", 480, 20, 20, PURPLE);
    EndDrawing();

    std::string result = karatsubaRec(x, y, 0, 600, 100, 400);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(("Result: "+x+" x "+y+" = "+result).c_str(), 400, 840, 22, PINK);
        EndDrawing();
    }

    CloseWindow();
}