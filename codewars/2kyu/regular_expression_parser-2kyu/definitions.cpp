#include <iostream>
#include "definitions.hpp"


RegExp* any () {
    RegExp *node = new RegExp;

    node->value = '.';
    node->left = nullptr;
    node->right = nullptr;

    return node;
}

RegExp* normal (char c) {
    std::cerr << "\tnormal: " << c << std::endl;
    RegExp *node = new RegExp;

    node->value = c;
    node->left = nullptr;
    node->right = nullptr;

    return node;
}

RegExp* zeroOrMore (RegExp *starred) {
    std::cerr << "\tzeroOrMore: " << starred->value << std::endl;
    RegExp *node = new RegExp;

    node->value = "zeroOrMore";
    node->left = starred;
    node->right = nullptr;

    return node;
}

RegExp* orr (RegExp *left, RegExp *right) {
    std::cerr << "\torr: " << left->value << " | " << right->value << std::endl;
    RegExp *node = new RegExp;

    node->value = "orr";
    node->left = left;
    node->right = right;

    return node;
}

RegExp* str (RegExp *first) {
    std::cerr << "\tstr: " << first->value << std::endl;
    RegExp *node = new RegExp;

    node->value = "str";
    node->left = first;
    node->right = nullptr;

    return node;
}

RegExp* add (RegExp *str, RegExp *next) {
    std::cerr << "\tadd: " << str->value << " + " << next->value << std::endl;
    RegExp *node = new RegExp;

    node->value = "add";
    node->left = str;
    node->right = next;

    return node;
}

void printRegExp(RegExp *expr) {
    if (expr == nullptr) {
        return;
    }

    if (expr->value == "orr") {
        printRegExp(expr->left);
        std::cout << "|";
        printRegExp(expr->right);
    } else if (expr->value == "zeroOrMore") {
        printRegExp(expr->left);
        std::cout << "[zeroOrMore]";
    } else if (expr->value == "str") {
        printRegExp(expr->left);
    } else if (expr->value == "add") {
        printRegExp(expr->left);
        std::cout << "+";
        printRegExp(expr->right);
    }
    else {
        std::cout << expr->value;
    }
}
