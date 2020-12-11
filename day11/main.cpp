#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

void printSeats(const std::vector<std::vector<char>>& seats) {
    for (const auto& row : seats) {
        for (char c : row) {
            std::cout << c << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int occupiedAdjacentSeats(int row, int col, const std::vector<std::vector<char>>& seats) {
    int nRows = seats.size();
    int nCols = seats.at(0).size();

    int res = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) {
                continue;
            }

            int consideredRow = row + i;
            int consideredCol = col + j;

            while ((consideredRow >= 0 && consideredRow < nRows) &&
                   (consideredCol >= 0 && consideredCol < nCols)) {
                if (seats.at(consideredRow).at(consideredCol) == '#') {
                    ++res;
                    break;
                } else if (seats.at(consideredRow).at(consideredCol) == 'L') {
                    break;
                } else {
                    consideredRow += i;
                    consideredCol += j;
                }
            }
        }
    }

    return res;
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    std::vector<std::vector<char>> seats{};
    while (std::getline(inputFile, buffer)) {
        std::vector<char> row{};
        for (char c : buffer) {
            row.push_back(c);
        }
        seats.push_back(row);
    }
    inputFile.close();

    int nChanges;

    do {
        nChanges = 0;
        printSeats(seats);
        std::vector<std::vector<char>> newSeats{};
        for (int row = 0; row < seats.size(); ++row) {
            std::vector<char> newRow{};
            for (int col = 0; col < seats.at(0).size(); ++col) {
                char currentSeat = seats.at(row).at(col);
                char newSeat;
                if (currentSeat == '.') {
                    newSeat = '.';
                } else {
                    int nOccupiedAdj = occupiedAdjacentSeats(row, col, seats);
                    if (currentSeat == 'L' && nOccupiedAdj == 0) {
                        newSeat = '#';
                        ++nChanges;
                    } else if (currentSeat == '#' && nOccupiedAdj >= 5) {
                        newSeat = 'L';
                        ++nChanges;
                    } else {
                        newSeat = currentSeat;
                    }
                }
                if (newSeat == '.' && currentSeat != '.') {
                    std::cout << currentSeat << std::endl;
                }
                newRow.push_back(newSeat);
            }
            newSeats.push_back(newRow);
        }
        seats = newSeats;
    } while (nChanges > 0);

    int nOccupiedSeats =
        std::accumulate(seats.cbegin(), seats.cend(), 0, [](int res, const std::vector<char>& row) {
            return res + std::count(row.cbegin(), row.cend(), '#');
        });

    std::cout << "Occupied seats once stabilized: " << nOccupiedSeats << std::endl;

    return 0;
}