#include <cmath>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int mod(int a, int b) {
    int ret = a % b;
    return ret >= 0 ? ret : ret + b;
}

class NavigationSimulator1 {
  public:
    int directionFaced;
    int posNorth;
    int posEast;

    NavigationSimulator1(int initialDirection, int initialPosNorth, int initialPosEast)
        : directionFaced(initialDirection), posNorth(initialPosNorth), posEast(initialPosEast) {}

    void executeAction(char action, int value) {
        switch (action) {
        case 'N':
            posNorth += value;
            break;

        case 'S':
            posNorth -= value;
            break;

        case 'E':
            posEast += value;
            break;

        case 'W':
            posEast -= value;
            break;

        case 'L':
            directionFaced = mod(directionFaced - value, 360);
            break;

        case 'R':
            directionFaced = mod(directionFaced + value, 360);
            break;

        case 'F':
            if (directionFaced == 0) {
                posNorth += value;
            } else if (directionFaced == 90) {
                posEast += value;
            } else if (directionFaced == 180) {
                posNorth -= value;
            } else if (directionFaced == 270) {
                posEast -= value;
            } else {
                throw std::runtime_error("Unexpected direction: " + std::to_string(directionFaced));
            }
            break;
        }
    }

    int manhattanDist() const { return std::abs(posEast) + std::abs(posNorth); }
};

class NavigationSimulator2 {
  public:
    int posNorth;
    int posEast;
    int waypointNorth;
    int waypointEast;

    NavigationSimulator2(int initialWaypointNorth, int initialWaypointEast)
        : posNorth(0), posEast(0), waypointNorth(initialWaypointNorth),
          waypointEast(initialWaypointEast) {}

    void executeAction(char action, int value) {
        switch (action) {
        case 'N':
            waypointNorth += value;
            break;

        case 'S':
            waypointNorth -= value;
            break;

        case 'E':
            waypointEast += value;
            break;

        case 'W':
            waypointEast -= value;
            break;

        case 'L':
            rotateWaypoint(360 - value);
            break;

        case 'R':
            rotateWaypoint(value);
            break;

        case 'F':
            posEast += value * waypointEast;
            posNorth += value * waypointNorth;
            break;
        }
    }

    void rotateWaypoint(int angle) {
        int prevWpNorth = waypointNorth;
        int prevWpEast = waypointEast;

        switch (angle) {
        case 90:
            waypointNorth = -prevWpEast;
            waypointEast = prevWpNorth;
            break;

        case 180:
            waypointNorth = -prevWpNorth;
            waypointEast = -prevWpEast;
            break;

        case 270:
            waypointNorth = prevWpEast;
            waypointEast = -prevWpNorth;
            break;
        }
    }

    int manhattanDist() const { return std::abs(posEast) + std::abs(posNorth); }
};

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;

    NavigationSimulator2 nav{1, 10};

    while (std::getline(inputFile, buffer)) {
        char action = buffer.at(0);
        int value = std::stoi(buffer.substr(1));

        nav.executeAction(action, value);
    }
    inputFile.close();

    std::cout << "Manhattan distance at the end: " << nav.manhattanDist() << std::endl;

    return 0;
}