#include <iostream>
#include <vector>

std::vector<int> the_lift(const std::vector<std::vector<int>> &queues, int capacity);

std::vector<std::pair<std::vector<std::vector<int>>, std::vector<int>>> tests = {
    {
        { {}, {}, {5,5,5}, {}, {}, {}, {} }, {0, 2, 5, 0}
    },
    {
        { {}, {}, {1,1}, {}, {}, {}, {} }, {0, 2, 1, 0}
    },
    {
        { {}, {3}, {4}, {}, {5}, {}, {} }, {0, 1, 2, 3, 4, 5, 0}
    },
    {
        { {}, {0}, {}, {}, {2}, {3}, {} }, {0, 5, 4, 3, 2, 1, 0}
    }
};

std::ostream &operator<<(std::ostream &os, const std::vector<int> &v) {
    for (int x : v) {
        os << x << " ";
    }
    return os;
}

int main() {
    for (const auto &test : tests) {
        const auto &queues = test.first;
        const auto &expected = test.second;
        std::vector<int> result = the_lift(queues, 5);

        if (result != expected) {
            std::cout << "Test failed: expected " << expected << ", got " << result << std::endl;
        } else {
            std::cout << "Test passed" << std::endl;
        }
    }
}
