#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    std::vector<std::string> inputs{};

    while (std::getline(inputFile, buffer)) {
        inputs.push_back(buffer);
    }
    inputFile.close();

    const int patternWidth = inputs.at(0).length();

    std::array<std::pair<int, int>, 5> slopes{{{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}}};
    int product = 1;

    for (auto slope : slopes) {
        int treesEncountered = 0;
        int horizontalPos = 0;
        int verticalPos = 0;

        do {
            if (inputs.at(verticalPos).at(horizontalPos % patternWidth) == '#') {
                ++treesEncountered;
            }
            horizontalPos += slope.first;
            verticalPos += slope.second;
        } while (verticalPos < inputs.size());

        product *= treesEncountered;
    }

    std::cout << product << std::endl;

    return 0;
}