#include <cctype>
#include <iostream>
#include <string>
#include "definitions.hpp"

RegExp *parseRegExp (const char *input);

int main() {
    std::string input = "ab|cd*";
    RegExp *result = parseRegExp(input.c_str());
    if (result != nullptr) {
        std::cout << "Parsed successfully" << std::endl;
        printRegExp(result);
        std::cout << std::endl;
    } else {
        std::cout << "Failed to parse" << std::endl;
    }
}
