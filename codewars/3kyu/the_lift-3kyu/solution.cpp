#include <set>
#include <queue>
#include <vector>
#include <iterator>
#include <algorithm>

using Person = int;

struct Direction {
    int value;

    static const int MAXDIR = 2;

    Direction(int value) : value(value) {}
    int operator()() const { return value; }

    bool operator==(const Direction &other) const { return value == other.value; }
    bool operator!=(const Direction &other) const { return value != other.value; }

    void flip() { value = 1 - value; }

    static Direction fromPath(int from, int to) {
        return to > from ? Direction(0) : Direction(1);
    }
} UP(0), DOWN(1);

struct Floor {
    int number;
    std::queue<Person> queue[Direction::MAXDIR];

    void loadQueue(const std::vector<int> &queue) {
        for (int dest : queue)
            this->queue[Direction::fromPath(number, dest)()].push(dest);
    }

    bool pressedButton(Direction dir) const {
        return !queue[dir()].empty();
    }
};

struct Lift {
    int currentFloor;
    Direction direction;
    int capacity;
    std::multiset<int> passengerDestinations;

    Lift(int capacity) : currentFloor(0), direction(UP), capacity(capacity) {}

    void removePassengers() {
        passengerDestinations.erase(currentFloor);
    }

    bool takePassanger(int floorDestination) {
        if (passengerDestinations.size() < capacity) {
            passengerDestinations.insert(floorDestination);
            return true;
        }
        return false;
    }

    bool isFull() const {
        return passengerDestinations.size() >= capacity;
    }

    bool isEmpty() const {
        return passengerDestinations.empty();
    }
};

struct Building {
    Lift lift;
    std::vector<Floor> floors;
    std::set<int> floorsWithPressedButtons[2];
    std::set<int> passengerDestinations;

    Building(int capacity, const std::vector<std::vector<int>> &queues)
        : lift(capacity) {
        floors.resize(queues.size());
        for (int i = 0; i < (int)queues.size(); ++i) {
            floors[i].number = i;
            floors[i].loadQueue(queues[i]);

            if (floors[i].pressedButton(UP))
                floorsWithPressedButtons[UP()].insert(i);
            if (floors[i].pressedButton(DOWN))
                floorsWithPressedButtons[DOWN()].insert(i);
        }
    }

    void processFloor() {
        lift.removePassengers();
        passengerDestinations.erase(lift.currentFloor);

        while (!lift.isFull() && !floors[lift.currentFloor].queue[lift.direction()].empty()) {
            int passenger = floors[lift.currentFloor].queue[lift.direction()].front();
            if (!lift.takePassanger(passenger))
                break;
            floors[lift.currentFloor].queue[lift.direction()].pop();
            passengerDestinations.insert(passenger);
        }

        if (floors[lift.currentFloor].queue[UP()].empty())
            floorsWithPressedButtons[UP()].erase(lift.currentFloor);
        if (floors[lift.currentFloor].queue[DOWN()].empty())
            floorsWithPressedButtons[DOWN()].erase(lift.currentFloor);
    }

    int nextUp(const std::set<int> &s) const {
        auto it = s.upper_bound(lift.currentFloor);
        return it != s.end() ? *it : -1;
    }

    int nextDown(const std::set<int> &s) const {
        auto it = s.lower_bound(lift.currentFloor);
        if (it == s.begin())
            return -1;
        return *std::prev(it);
    }

    int maxAtOrAbove(const std::set<int> &s) const {
        if (s.empty())
            return -1;
        auto it = s.lower_bound(lift.currentFloor);
        if (it == s.end())
            return -1;
        return *s.rbegin();
    }

    int minAtOrBelow(const std::set<int> &s) const {
        if (s.empty())
            return -1;
        auto it = s.upper_bound(lift.currentFloor);
        if (it == s.begin())
            return -1;
        return *s.begin();
    }

    int closestSameDirectionStop(Direction dir) const {
        if (dir == UP) {
            int call = nextUp(floorsWithPressedButtons[UP()]);
            int dest = nextUp(passengerDestinations);
            if (call == -1) return dest;
            if (dest == -1) return call;
            return std::min(call, dest);
        } else {
            int call = nextDown(floorsWithPressedButtons[DOWN()]);
            int dest = nextDown(passengerDestinations);
            if (call == -1) return dest;
            if (dest == -1) return call;
            return std::max(call, dest);
        }
    }

    int moveLift() {
        int stop = closestSameDirectionStop(lift.direction);
        if (stop != -1)
            return lift.currentFloor = stop;

        if (lift.isEmpty()) {
            if (lift.direction == UP) {
                int smart = maxAtOrAbove(floorsWithPressedButtons[DOWN()]);
                if (smart != -1) {
                    if (smart == lift.currentFloor)
                        lift.direction.flip();
                    return lift.currentFloor = smart;
                }
            } else {
                int smart = minAtOrBelow(floorsWithPressedButtons[UP()]);
                if (smart != -1) {
                    if (smart == lift.currentFloor)
                        lift.direction.flip();
                    return lift.currentFloor = smart;
                }
            }
        }

        lift.direction.flip();

        if (!floors[lift.currentFloor].queue[lift.direction()].empty())
            return lift.currentFloor;

        stop = closestSameDirectionStop(lift.direction);
        if (stop != -1)
            return lift.currentFloor = stop;

        return lift.currentFloor;
    }

    bool isEmpty() const {
        return floorsWithPressedButtons[UP()].empty() &&
               floorsWithPressedButtons[DOWN()].empty() &&
               lift.isEmpty();
    }
};

std::vector<int> the_lift(const std::vector<std::vector<int>> &queues, int capacity) {
    std::vector<int> result = {0};

    Building building(capacity, queues);
    while (!building.isEmpty()) {
        building.processFloor();
        if (building.isEmpty())
            break;

        int prevFloor = building.lift.currentFloor;
        Direction prevDir = building.lift.direction;
        int newFloor = building.moveLift();

        if (newFloor == prevFloor && building.lift.direction == prevDir)
            break;

        if (newFloor != prevFloor)
            result.push_back(newFloor);
    }

    if (!result.empty() && result.back() != 0)
        result.push_back(0);
    return result;
}
