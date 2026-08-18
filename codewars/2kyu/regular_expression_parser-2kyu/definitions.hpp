#pragma once
#include <string>

struct RegExp {
    std::string value;
    RegExp *left, *right;
};

void printRegExp(RegExp *expr);
