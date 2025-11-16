#include "raylib.h"
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
using namespace std;

static void sleepMs(int ms) { this_thread::sleep_for(chrono::milliseconds(ms)); }

string padLeft(const string &s, int n) {
    return string(n - s.size(), '0') + s;
}

string addStr(const string &a, const string &b) {
    int n = max(a.size(), b.size());
    string A = padLeft(a, n), B = padLeft(b, n);
    string res(n+1, '0');
    int carry = 0;
    for (int i = n-1; i >= 0; i--) {
        int sum = (A[i]-'0') + (B[i]-'0') + carry;
        res[i+1] = (sum%10) + '0';
        carry = sum/10;
    }
    if (carry) res[0] = carry+'0';
    return res[0]=='0' ? res.substr(1) : res;
}

string subStr(const string &a, const string &b) {
    int n = a.size();
    string res(n, '0');
    int borrow = 0;
    for (int i = n-1; i >= 0; i--) {
        int diff = (a[i]-'0') - (b[i]-'0') - borrow;
        if (diff < 0) diff += 10, borrow = 1;
        else borrow = 0;
        res[i] = diff+'0';
    }
    size_t start = res.find_first_not_of('0');
    return start==string::npos ? "0" : res.substr(start);
}

string multStrSmall(const string &a, const string &b) {
    long long x = stoll(a);
    long long y = stoll(b);
    return to_string(x*y);
}

// for logs to display later
vector<string> logs;

void logStep(const string &msg) {
    logs.push_back(msg);
    sleepMs(150);
}

string karatsubaRec(const string &x, const string &y, int depth = 0) {
    string indent(depth * 2, ' ');
    logStep(indent + "karatsuba (" + x + ", \n\t" + y + ")");

    if (x.size() <= 4 || y.size() <= 4) {
        string res = multStrSmall(x, y);
        logStep(indent + ">>  base case: " + x + " × " + y + " = " + res);
        return res;
    }

    int n = max(x.size(), y.size());
    int m = n / 2;
    string xPad = padLeft(x, n);
    string yPad = padLeft(y, n);

    string high1 = xPad.substr(0, n - m);
    string low1  = xPad.substr(n - m);
    string high2 = yPad.substr(0, n - m);
    string low2  = yPad.substr(n - m);

    logStep(indent + "split:");
    logStep(indent + "  x = (" + high1 + ", " + low1 + ")");
    logStep(indent + "  y = (" + high2 + ", " + low2 + ")");

    // temporary stacks to visualize recursion
    string z2 = karatsubaRec(high1, high2, depth + 1);
    string z0 = karatsubaRec(low1, low2, depth + 1);
    string z1 = karatsubaRec(addStr(high1, low1), addStr(high2, low2), depth + 1);

    // z1 = (high1 + low1)(high2 + low2) - z2 - z0
    string z2shift = z2 + string(2 * m, '0');
    string z1shift = subStr(subStr(z1, z2), z0) + string(m, '0');
    string res = addStr(addStr(z2shift, z1shift), z0);

    logStep(indent + "combine: \n" + indent + "z2 = " + z2 + ", \n" + indent + "z1 = " + z1 + ",\n" + indent + "z0 = " + z0);
    logStep(indent + ">> >>  result = " + res);

    return res;
}

void visualizeKaratsuba(const string &x, const string &y, float scroll) {
    int yPos = 60 - static_cast<int>(scroll); // scroll applied here

    for (size_t i = 0; i < logs.size(); i++) {
        string log = logs[i];
        size_t start = 0, end;

        while ((end = log.find('\n', start)) != string::npos) {
            string line = log.substr(start, end - start);
            DrawText(line.c_str(), 40, yPos, 18, LIGHTGRAY);
            yPos += 25; // spacing between multi-line text
            start = end + 1;
        }

        string line = log.substr(start);
        DrawText(line.c_str(), 40, yPos, 18, LIGHTGRAY);
        yPos += 45; // extra spacing between logs
    }
}