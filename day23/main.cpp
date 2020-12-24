#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// struct Cup {
//     int value;
//     Cup* following;

//     Cup() : value(0) {}
//     Cup(int value) : value(value) {}
// };

// void printCups(Cup* firstCup) {
//     Cup* nextCup = firstCup->following;
//     std::cout << firstCup->value;
//     while (nextCup != firstCup) {
//         std::cout << " -> " << nextCup->value;
//         nextCup = nextCup->following;
//     }
//     std::cout << std::endl;
// }

int mod(int a, int b) {
    int ret = a % b;
    return ret >= 0 ? ret : ret + b;
}

int decrementCup(int cup) { return mod(cup - 2, 1'000'000) + 1; }

int getDestinationCup(int currentCup, const std::vector<int>& cups) {
    int destinationCup = decrementCup(currentCup);

    while (cups[currentCup] == destinationCup || cups[cups[currentCup]] == destinationCup ||
           cups[cups[cups[currentCup]]] == destinationCup) {
        destinationCup = decrementCup(destinationCup);
    }

    return destinationCup;
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;

    std::getline(inputFile, buffer);
    inputFile.close();

    // Using a vector instead of a map to store cups for faster access
    // Size 1'000'001 so that we don't have to use (value - 1) as index each time
    // The place at index 0 is unused
    std::vector<int> cups(1'000'001);

    int previousCup;
    int firstCup;

    // Initializing the cups contained in the input
    for (int i = 0; i < buffer.size(); ++i) {
        int value = static_cast<int>(buffer[i] - '0');

        if (i == 0) {
            firstCup = value;
        }
        if (i > 0) {
            cups[previousCup] = value;
        }

        previousCup = value;
    }

    // Initializing the other cups from 10 to 1'000'000
    for (int i = 10; i <= 1'000'000; ++i) {
        cups[previousCup] = i;
        previousCup = i;
    }
    cups[1'000'000] = firstCup;

    // Playing the actual 10'000'000 rounds
    int currentCup = firstCup;
    for (int i = 0; i < 10'000'000; ++i) {
        // std::cout << "Before move " << i << ": ";
        // printCups(currentCup);

        // Choosing destination cup and removing the 3 cups after the currentCup from the circle
        int firstOf3Cups = cups[currentCup];
        int lastOf3Cups = cups[cups[firstOf3Cups]];

        int destinationCup = getDestinationCup(currentCup, cups);

        cups[currentCup] = cups[lastOf3Cups];

        // Reinserting them after the destinationCup
        cups[lastOf3Cups] = cups[destinationCup];
        cups[destinationCup] = firstOf3Cups;

        currentCup = cups[currentCup];
    }

    // The cast is needed to prevent integer overflow with int multiplication
    long result = static_cast<long>(cups[1]) * cups[cups[1]];

    std::cout << "Product of the two cups clockwise of cup 1: " << result << std::endl;

    return 0;
}