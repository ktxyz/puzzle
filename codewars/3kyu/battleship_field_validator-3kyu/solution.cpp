#include <algorithm>
#include <vector>


int remove_ship(std::vector<std::vector<int>>& field, int y, int x) {
    auto is_empty = [&field](int y, int x) {
        return y < 0 || y >= field.size() || x < 0 || x >= field[y].size() || field[y][x] == 0;
    };

    auto corners_are_empty = [&is_empty](int y, int x) {
        return is_empty(y - 1, x - 1) && is_empty(y + 1, x + 1) && is_empty(y + 1, x - 1) && is_empty(y - 1, x + 1);
    };

    auto left_right_are_empty = [&is_empty](int y, int x) {
        return is_empty(y, x - 1) && is_empty(y, x + 1);
    };

    auto top_bottom_are_empty = [&is_empty](int y, int x) {
        return is_empty(y - 1, x) && is_empty(y + 1, x);
    };

    auto count_vertical_ship = [&is_empty, &corners_are_empty](int y, int x) {
        int ship_pieces = 0;
        for (auto iy = y; !is_empty(iy, x); ++iy) {
            ++ship_pieces;
            if (!corners_are_empty(iy, x))
                return -1;
        }
        return ship_pieces;
    };

    auto count_horizontal_ship = [&is_empty, &corners_are_empty](int y, int x) {
        int ship_pieces = 0;
        for (auto ix = x; !is_empty(y, ix); ++ix) {
            ++ship_pieces;
            if (!corners_are_empty(y, ix))
                return -1;
        }
        return ship_pieces;
    };

    auto vert_ship = count_vertical_ship(y, x);
    auto horz_ship = count_horizontal_ship(y, x);

    if ((vert_ship > 1 && horz_ship > 1) || vert_ship == -1 || horz_ship == -1)
        return -1;

    if (vert_ship > 1) {
        for (auto iy = y; !is_empty(iy, x); ++iy) {
            if (!left_right_are_empty(iy, x))
                return -1;
            field[iy][x] = 0;
        }
        return vert_ship;
    } else {
        for (auto ix = x; !is_empty(y, ix); ++ix) {
            if (!top_bottom_are_empty(y, ix))
                return -1;
            field[y][ix] = 0;
        }
        return horz_ship;
    }

    return std::max(vert_ship, horz_ship);
}

bool validate_battlefield(std::vector<std::vector<int>> field) {
    std::array<int, 4> ship_amounts = {4, 3, 2, 1};

    for (auto y = 0; y < field.size(); ++y) {
        for (auto x = 0; x < field[y].size(); ++x) {
            if (field[y][x] == 1) {
                auto ship_size = remove_ship(field, y, x);
                if (ship_size == -1) {
                    return false;
                } else if (--ship_size >= ship_amounts.size()) {
                    return false;
                }
                --ship_amounts[ship_size];
            }
        }
    }

    return std::all_of(ship_amounts.begin(), ship_amounts.end(), [](int amount) { return amount == 0; });
}
