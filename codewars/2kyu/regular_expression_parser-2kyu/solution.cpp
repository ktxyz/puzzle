#ifdef _LOCAL
#include "definitions.hpp"
#endif

#include <unordered_map>
#include <vector>
#include <cstring>

RegExp* any ();
RegExp* normal (char c);
RegExp* zeroOrMore (RegExp *starred);
RegExp* orr (RegExp *left, RegExp *right);
RegExp* str (RegExp *first);
RegExp* add (RegExp *str, RegExp *next);

#define ret_null_if_null(x) if ((x) == nullptr) return nullptr;

enum ERegFlag : bool {
    REG_FLAG_NONE = 0,
    REG_FLAG_STARRED = 1,
};

RegExp* parseRegExpGroup(std::string& str_input, size_t i, std::unordered_map<size_t, RegExp*> &group_cache,
                                std::unordered_map<size_t, size_t> &group_end, std::vector<ERegFlag> &zomFlags) {
    if (i >= str_input.length()) return nullptr;
    if (str_input[i] != '(') return nullptr;

    std::vector<RegExp*> group_blocks;
    RegExp *curr_block = nullptr;
    bool is_curr_seq = false;

    for (size_t j = i + 1; j < group_end[i]; j++) {
        char c = str_input[j];
        if (c == '*') continue;
        else if (c == ')') continue;

        RegExp* atom = nullptr;
        switch (c) {
            case '(':
                atom = group_cache[j];
                j = group_end[j];
                break;
            case '.':
                atom = any();
                break;
            case '|':
                if (curr_block == nullptr) return nullptr;
                group_blocks.push_back(curr_block);
                curr_block = nullptr;
                is_curr_seq = false;
                continue;
                break;
            default:
                atom = normal(c);
                break;
        }

        if (zomFlags[j] & REG_FLAG_STARRED) atom = zeroOrMore(atom);

        if (curr_block == nullptr) {
            curr_block = atom;
        } else {
            if (!is_curr_seq) {
                curr_block = str(curr_block);
                is_curr_seq = true;
            }
            curr_block = add(curr_block, atom);
        }
    }
    if (curr_block != nullptr) group_blocks.push_back(curr_block);

    if (group_blocks.empty()) return nullptr;
    for(auto i = 1; i < group_blocks.size(); i++) {
        group_blocks[0] = orr(group_blocks[0], group_blocks[i]);
    }
    return group_blocks[0];
}

bool splitIntoGroups(std::string& str_input, std::vector<std::pair<size_t, size_t>>& groups) {
    std::vector<size_t> parenStack;

    for (size_t i = 0; i < str_input.length(); i++) {
        char c = str_input[i];

        switch (c) {
            case '(':
                parenStack.push_back(i);
                break;
            case ')':
                if (parenStack.empty()) return false;

                groups.push_back(std::make_pair(parenStack.back(), i));
                parenStack.pop_back();
                break;
            case '*':
                if (i == 0) return false;
                if (str_input[i - 1] == '|' || str_input[i - 1] == '(') return false;
                break;
            case '|':
                if (i == 0 || i == str_input.length() - 1) return false;
                if (str_input[i - 1] == '|' || str_input[i + 1] == '|') return false;
                break;
            default:
                break;
        }
    }

    if (!parenStack.empty()) return false;
    return true;
}

RegExp *parseRegExp (const char *input) {
    if (strlen(input) == 0) return nullptr;

    std::string str_input = "(";
    str_input += input;
    str_input += ")";

    std::vector<std::pair<size_t, size_t>> groups;
    if(!splitIntoGroups(str_input, groups)) return nullptr;

    std::vector<ERegFlag> zomFlags(str_input.size(), REG_FLAG_NONE);
    for(size_t i = 0; i < str_input.size(); i++) {
        if (str_input[i] == '*') {
            if (i == 0 || str_input[i - 1] == '|' || str_input[i - 1] == '(' || str_input[i - 1] == '*') return nullptr;
            zomFlags[i - 1] = REG_FLAG_STARRED;
        }
    }

    std::unordered_map<size_t, RegExp*> group_cache;
    {
        std::unordered_map<size_t, size_t> group_end;
        for (auto &group : groups) {
            group_end[group.first] = group.second;
            RegExp* groupRegExpr = parseRegExpGroup(str_input, group.first, group_cache, group_end, zomFlags);
            ret_null_if_null(groupRegExpr);
            group_cache[group.first] = groupRegExpr;
        }
    }

    if (group_cache.empty()) return nullptr;
    return group_cache[0];
}
