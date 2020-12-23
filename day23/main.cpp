#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

struct Cup {
    int value;
    Cup* following;

    Cup() : value(0) {}
    Cup(int value) : value(value) {}
};

void printCups(Cup* firstCup) {
    Cup* nextCup = firstCup->following;
    std::cout << firstCup->value;
    while (nextCup != firstCup) {
        std::cout << " -> " << nextCup->value;
        nextCup = nextCup->following;
    }
    std::cout << std::endl;
}

int mod(int a, int b) {
    int ret = a % b;
    return ret >= 0 ? ret : ret + b;
}

int decrementLabel(int label) { return mod(label - 2, 1'000'000) + 1; }

int getDestinationLabel(int currentLabel, Cup* firstOf3Cups) {
    int destinationLabel = decrementLabel(currentLabel);
    while (firstOf3Cups->value == destinationLabel ||
           firstOf3Cups->following->value == destinationLabel ||
           firstOf3Cups->following->following->value == destinationLabel) {
        destinationLabel = decrementLabel(destinationLabel);
    }

    return destinationLabel;
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;

    std::getline(inputFile, buffer);
    inputFile.close();

    // Using a vector instead of a map to store cups for faster access
    std::vector<Cup> cups(1'000'000);

    Cup* previousCup{nullptr};
    Cup* firstCup{nullptr};

    // Initializing the cups contained in the input
    for (int i = 0; i < buffer.size(); ++i) {
        int value = static_cast<int>(buffer[i] - '0');
        cups[value - 1] = Cup(value);
        Cup* cupPtr = &(cups[value - 1]);

        if (i == 0) {
            firstCup = cupPtr;
        }
        if (i > 0) {
            previousCup->following = cupPtr;
        }

        previousCup = cupPtr;
    }

    // Initializing the other cups from 10 to 1'000'000
    for (int i = 10; i <= 1'000'000; ++i) {
        cups[i - 1] = Cup(i);
        Cup* cupPtr = &(cups[i - 1]);

        if (i > 0) {
            previousCup->following = cupPtr;
        }

        previousCup = cupPtr;
    }
    cups[1'000'000 - 1].following = firstCup;

    // Playing the actual 10'000'000 rounds
    Cup* currentCup = firstCup;
    for (int i = 0; i < 10'000'000; ++i) {
        // std::cout << "Before move " << i << ": ";
        // printCups(currentCup);

        // Removing the 3 cups after the currentCup from the circle
        Cup* firstOf3Cups = currentCup->following;
        Cup* lastOf3Cups = currentCup->following->following->following;

        currentCup->following = lastOf3Cups->following;

        // Choosing destination cup
        int destinationLabel = getDestinationLabel(currentCup->value, firstOf3Cups);
        Cup* destinationCup = &(cups[destinationLabel - 1]);

        // Reinserting them after the destinationCup
        lastOf3Cups->following = destinationCup->following;
        destinationCup->following = firstOf3Cups;

        currentCup = currentCup->following;
    }

    Cup cupOne = cups[0];
    // The cast is needed to prevent integer overflow with int multiplication
    long result = static_cast<long>(cupOne.following->value) * cupOne.following->following->value;

    std::cout << "Product of the two cups clockwise of cup 1: " << result << std::endl;

    return 0;
}