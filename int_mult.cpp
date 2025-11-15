#include "raylib.h"
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>

static void sleepMs(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

std::string padLeft(const std::string &s, int n) {
    return std::string(n - s.size(), '0') + s;
}
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
std::string subStr(const std::string &a, const std::string &b) {
    int n = a.size();
    std::string res(n, '0');
    int borrow = 0;
    for (int i = n-1; i >= 0; i--) {
        int diff = (a[i]-'0') - (b[i]-'0') - borrow;
        if (diff < 0) diff += 10, borrow = 1;
        else borrow = 0;
        res[i] = diff+'0';
    }
    size_t start = res.find_first_not_of('0');
    return start==std::string::npos ? "0" : res.substr(start);
}
std::string multStrSmall(const std::string &a, const std::string &b) {
    long long x = std::stoll(a);
    long long y = std::stoll(b);
    return std::to_string(x*y);
}

// logs to display later
std::vector<std::string> logs;

void logStep(const std::string &msg) {
    logs.push_back(msg);
    sleepMs(150);
}

std::string karatsubaRec(const std::string &x, const std::string &y, int depth = 0) {
    std::string indent(depth * 2, ' ');
    logStep(indent + "karatsuba (" + x + ", \n\t" + y + ")");

    if (x.size() <= 4 || y.size() <= 4) {
        std::string res = multStrSmall(x, y);
        logStep(indent + ">>  base case: " + x + " × " + y + " = " + res);
        return res;
    }

    int n = std::max(x.size(), y.size());
    int m = n / 2;
    std::string xPad = padLeft(x, n);
    std::string yPad = padLeft(y, n);

    std::string high1 = xPad.substr(0, n - m);
    std::string low1  = xPad.substr(n - m);
    std::string high2 = yPad.substr(0, n - m);
    std::string low2  = yPad.substr(n - m);

    logStep(indent + "split:");
    logStep(indent + "  x = (" + high1 + ", " + low1 + ")");
    logStep(indent + "  y = (" + high2 + ", " + low2 + ")");

    std::string z2 = karatsubaRec(high1, high2, depth + 1);
    std::string z0 = karatsubaRec(low1, low2, depth + 1);
    std::string z1 = karatsubaRec(addStr(high1, low1), addStr(high2, low2), depth + 1);

    std::string z2shift = z2 + std::string(2 * m, '0');
    std::string z1shift = subStr(subStr(z1, z2), z0) + std::string(m, '0');
    std::string res = addStr(addStr(z2shift, z1shift), z0);

    logStep(indent + "combine: \n" + indent + "z2 = " + z2 + ", \n" + indent + "z1 = " + z1 + ",\n" + indent + "z0 = " + z0);
    logStep(indent + ">> >>  result = " + res);

    return res;
}

void visualizeKaratsuba(const std::string &x, const std::string &y, float scroll)
{
    int yPos = 60 - static_cast<int>(scroll); // scroll applied here

    for (size_t i = 0; i < logs.size(); i++)
    {
        std::string log = logs[i];
        size_t start = 0, end;

        while ((end = log.find('\n', start)) != std::string::npos)
        {
            std::string line = log.substr(start, end - start);
            DrawText(line.c_str(), 40, yPos, 18, LIGHTGRAY);
            yPos += 25; // spacing between multi-line text
            start = end + 1;
        }

        std::string line = log.substr(start);
        DrawText(line.c_str(), 40, yPos, 18, LIGHTGRAY);
        yPos += 45; // extra spacing between logs
    }
}