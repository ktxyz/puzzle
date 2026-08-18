#include <iostream>
#include <utility>
#include <vector>

bool validate_battlefield(std::vector<std::vector<int>> field);


auto tests = {
    std::pair<bool, std::vector<std::vector<int>>> {true, {
          std::vector<int> {1, 0, 0, 0, 0, 1, 1, 0, 0, 0},
          std::vector<int> {1, 0, 1, 0, 0, 0, 0, 0, 1, 0},
          std::vector<int> {1, 0, 1, 0, 1, 1, 1, 0, 1, 0},
          std::vector<int> {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          std::vector<int> {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
          std::vector<int> {0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
          std::vector<int> {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
          std::vector<int> {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
          std::vector<int> {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
          std::vector<int> {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        }}
};

int main() {
    for (auto& test : tests) {
        if (validate_battlefield(test.second) != test.first) {
            std::cout << "Test failed: " << test.first << std::endl;
        } else {
            std::cout << "Test passed: " << test.first << std::endl;
        }
    }
    return 0;
}
