#pragma once
#include <string>

extern vector<string> logs;

void visualizeKaratsuba(const string &x, const string &y, float scroll);
void logStep(const string &msg);
string karatsubaRec(const string &x, const string &y, int depth = 0);