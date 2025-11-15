#pragma once
#include <string>

extern std::vector<std::string> logs;

void visualizeKaratsuba(const std::string &x, const std::string &y, float scroll);
void logStep(const std::string &msg);
std::string karatsubaRec(const std::string &x, const std::string &y, int depth = 0);