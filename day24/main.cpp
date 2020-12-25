#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// A regular hex grid would have spacings on the Y axis of sqrt(3),
// but using this system causes floating point errors when computing coordinates.
// Instead, we use a "skewed" hex grid, where the hexagons are stretched along the X axis
// so that all steps on the grid are integers.
// So we have a spacing of (2, 0) between a hexagon and its E/W neighbor, and (1, 1) between a
// hexagon and its NE/NW/SE/SW neighbor.
const std::array<std::pair<int, int>, 6> NEIGHBOR_OFFSETS{
    {{-2, 0}, {2, 0}, {-1, 1}, {1, 1}, {-1, -1}, {1, -1}}};

struct PairOfintsHash {
    size_t operator()(const std::pair<int, int>& v) const {
        auto hasher = std::hash<int>();
        return hasher(v.first) ^ hasher(v.second);
    }
};

using TileSet = std::unordered_set<std::pair<int, int>, PairOfintsHash>;

std::vector<std::pair<int, int>> getNeighbors(const std::pair<int, int>& tile) {
    std::vector<std::pair<int, int>> res{};
    for (int i = 0; i < 6; ++i) {
        auto offset = NEIGHBOR_OFFSETS[i];
        res.push_back(std::pair<int, int>(tile.first + offset.first, tile.second + offset.second));
    }

    return res;
}

int countBlackNeighbors(const std::pair<int, int>& tile, const TileSet& blackTiles) {
    int blackNeighbors = 0;
    auto neighbors = getNeighbors(tile);
    for (const auto& neighbor : neighbors) {
        if (blackTiles.count(neighbor)) {
            ++blackNeighbors;
        }
    }

    return blackNeighbors;
}

TileSet iterateFlipping(const TileSet& blackTiles) {
    TileSet nextBlackTiles{};
    TileSet seenTiles{};

    for (const auto& tile : blackTiles) {
        auto neighbors = getNeighbors(tile);
        neighbors.push_back(tile);
        for (const auto& neighbor : neighbors) {
            if (seenTiles.count(neighbor)) {
                continue;
            }

            int blackNeighbors = countBlackNeighbors(neighbor, blackTiles);
            if (!blackTiles.count(neighbor) && blackNeighbors == 2) {
                nextBlackTiles.insert(neighbor);
            } else if (blackTiles.count(neighbor) && (blackNeighbors == 1 || blackNeighbors == 2)) {
                nextBlackTiles.insert(neighbor);
            }
            seenTiles.insert(neighbor);
        }
    }

    return nextBlackTiles;
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    TileSet blackTiles{};

    while (std::getline(inputFile, buffer)) {
        int x = 0;
        int y = 0;

        auto it = buffer.begin();
        auto end = buffer.end();

        while (it != end) {
            if (*it == 'w') {
                x -= 2;
            } else if (*it == 'e') {
                x += 2;
            } else {
                if (*it == 'n') {
                    y += 1;
                } else if (*it == 's') {
                    y -= 1;
                }
                ++it;
                if (*it == 'w') {
                    x -= 1;
                } else if (*it == 'e') {
                    x += 1;
                }
            }
            ++it;
        }

        std::pair<int, int> tilePos{x, y};

        if (blackTiles.count(tilePos)) {
            blackTiles.erase(tilePos);
        } else {
            blackTiles.insert(tilePos);
        }
    }

    inputFile.close();

    std::cout << "Number of black tiles at day 0: " << blackTiles.size() << std::endl;

    for (int i = 1; i <= 100; ++i) {
        blackTiles = iterateFlipping(blackTiles);
        std::cout << "Number of black tiles at day " << i << ": " << blackTiles.size() << std::endl;
    }

    return 0;
}