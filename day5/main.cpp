#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;

    int maxSeatId = 0;
    std::unordered_set<int> presentIds{};

    while (std::getline(inputFile, buffer)) {
        unsigned char row = 0;
        for (int i = 0; i < 7; ++i) {
            row <<= 1;
            if (buffer.at(i) == 'B') {
                ++row;
            }
        }

        unsigned char column = 0;
        for (int i = 7; i < 10; ++i) {
            column <<= 1;
            if (buffer.at(i) == 'R') {
                ++column;
            }
        }

        int seatId = 8 * row + column;
        presentIds.insert(seatId);
        if (seatId > maxSeatId) {
            maxSeatId = seatId;
        }
    }
    inputFile.close();

    std::cout << "Max seat ID: " << maxSeatId << std::endl;

    for (int id = 1; id < maxSeatId; ++id) {
        if (presentIds.count(id - 1) && !presentIds.count(id) && presentIds.count(id + 1)) {
            std::cout << "Your seat ID is: " << id << std::endl;
            break;
        }
    }

    return 0;
}