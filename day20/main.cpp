#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using Pixels = std::vector<std::vector<char>>;

std::vector<std::pair<int, int>> MONSTER_HASH_SIGNS_POS{{{0, 18},
                                                         {1, 0},
                                                         {1, 5},
                                                         {1, 6},
                                                         {1, 11},
                                                         {1, 12},
                                                         {1, 17},
                                                         {1, 18},
                                                         {1, 19},
                                                         {2, 1},
                                                         {2, 4},
                                                         {2, 7},
                                                         {2, 10},
                                                         {2, 13},
                                                         {2, 16}}};
std::pair<int, int> MONSTER_DIMENSIONS{3, 20};

void transform(Pixels& pixels, std::function<std::pair<int, int>(int, int)> transformFn) {
    Pixels newPixels{};
    for (int i = 0; i < pixels.size(); ++i) {
        std::vector<char> row{};
        for (int j = 0; j < pixels.size(); ++j) {
            auto srcPixPos = transformFn(i, j);
            row.push_back(pixels[srcPixPos.first][srcPixPos.second]);
        }
        newPixels.push_back(row);
    }
    pixels = newPixels;
}

void rotateRight(Pixels& pixels) {
    transform(pixels, [&pixels](int i, int j) { return std::pair(pixels.size() - 1 - j, i); });
}

void flipHoriz(Pixels& pixels) {
    transform(pixels, [&pixels](int i, int j) { return std::pair(i, pixels.size() - 1 - j); });
}

void flipVert(Pixels& pixels) {
    transform(pixels, [&pixels](int i, int j) { return std::pair(pixels.size() - 1 - i, j); });
}

bool flipUntilTrue(Pixels& pixels, std::function<bool()> predicate) {
    if (predicate()) {
        return true;
    }
    flipHoriz(pixels);
    if (predicate()) {
        return true;
    }
    flipVert(pixels);
    if (predicate()) {
        return true;
    }
    flipHoriz(pixels);
    if (predicate()) {
        return true;
    }
    return false;
}

struct Tile {
    Pixels pixels;
    int id;
    Tile(int id) : pixels(), id(id) {}

    void addRow(std::vector<char> row) { pixels.push_back(row); }

    std::vector<char> topEdge() const { return pixels[0]; }

    std::vector<char> bottomEdge() const { return pixels[9]; }

    std::vector<char> leftEdge() const {
        std::vector<char> res{};
        for (int i = 0; i < 10; ++i) {
            res.push_back(pixels[i][0]);
        }
        return res;
    }

    std::vector<char> rightEdge() const {
        std::vector<char> res{};
        for (int i = 0; i < 10; ++i) {
            res.push_back(pixels[i][9]);
        }
        return res;
    }

    bool linesUpAny(Tile& other) const {
        for (auto edge : {bottomEdge(), topEdge(), leftEdge(), rightEdge()}) {
            if (flipUntilTrue(other.pixels,
                              [&other, &edge]() { return other.matchesTopOrRight(edge); })) {
                return true;
            }
        }

        return false;
    }

    bool existsThatLinesUpBottomAndRight(std::vector<Tile>& otherTiles) const {
        for (auto edge : {bottomEdge(), rightEdge()}) {
            bool matched = false;
            for (Tile& other : otherTiles) {
                if (other.id == id) {
                    continue;
                }
                if (flipUntilTrue(other.pixels,
                                  [&other, &edge]() { return other.matchesTopOrRight(edge); })) {
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                return false;
            }
        }
        return true;
    }

    bool linesUpRight(Tile& other) const {
        auto rEdge = rightEdge();
        if (flipUntilTrue(other.pixels, [&other, &rEdge]() { return other.leftEdge() == rEdge; })) {
            return true;
        }

        rotateRight(other.pixels);

        if (flipUntilTrue(other.pixels, [&other, &rEdge]() { return other.leftEdge() == rEdge; })) {
            return true;
        }

        return false;
    }

    bool linesUpBottom(Tile& other) const {
        auto bEdge = bottomEdge();
        if (flipUntilTrue(other.pixels, [&other, &bEdge]() { return other.topEdge() == bEdge; })) {
            return true;
        }

        rotateRight(other.pixels);

        if (flipUntilTrue(other.pixels, [&other, &bEdge]() { return other.topEdge() == bEdge; })) {
            return true;
        }

        return false;
    }

    Tile findTheOneThatLinesUpRight(std::vector<Tile>& otherTiles) const {
        for (Tile& other : otherTiles) {
            if (other.id != id) {
                if (linesUpRight(other)) {
                    return other;
                }
            }
        }
        throw std::runtime_error("Did not find a tile that lines up right for tile " +
                                 std::to_string(id));
    }

    Tile findTheOneThatLinesUpBottom(std::vector<Tile>& otherTiles) const {
        for (Tile& other : otherTiles) {
            if (other.id != id) {
                if (linesUpBottom(other)) {
                    return other;
                }
            }
        }
        throw std::runtime_error("Did not find a tile that lines up right for tile " +
                                 std::to_string(id));
    }

  private:
    bool matchesTopOrRight(const std::vector<char>& edge) {
        return rightEdge() == edge || topEdge() == edge;
    }
};

void printIds(const std::vector<std::vector<Tile>>& assembledTiles) {
    for (const auto& row : assembledTiles) {
        for (const Tile& tile : row) {
            std::cout << tile.id << ' ';
        }
        std::cout << std::endl;
    }
}

Pixels toImage(const std::vector<std::vector<Tile>>& assembledTiles) {
    std::vector<std::vector<char>> img{};
    for (const auto& tileRow : assembledTiles) {
        for (int i = 1; i < 9; ++i) {
            std::vector<char> imgLine{};
            for (const Tile& tile : tileRow) {
                for (int j = 1; j < 9; ++j) {
                    imgLine.push_back(tile.pixels[i][j]);
                }
            }
            img.push_back(imgLine);
        }
    }
    return img;
}

void printImage(const Pixels& img) {
    for (const auto& line : img) {
        for (char c : line) {
            std::cout << c << ' ';
        }
        std::cout << std::endl;
    }
}

int countSeaMonsters(Pixels& image) {
    int nMonsters = 0;
    for (int i = 0; i < image.size() - MONSTER_DIMENSIONS.first; ++i) {
        for (int j = 0; j < image.size() - MONSTER_DIMENSIONS.second; ++j) {
            bool isMonster = true;
            for (const auto& pos : MONSTER_HASH_SIGNS_POS) {
                if (image[i + pos.first][j + pos.second] != '#') {
                    isMonster = false;
                    break;
                }
            }
            if (isMonster) {
                ++nMonsters;
            }
        }
    }
    return nMonsters;
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    std::vector<Tile> tiles{};
    std::getline(inputFile, buffer);

    Tile currentTile{std::stoi(buffer.substr(5, 4))};

    while (std::getline(inputFile, buffer)) {
        if (buffer == "") {
            tiles.push_back(currentTile);
            std::getline(inputFile, buffer);
            int id = std::stoi(buffer.substr(5, 4));
            currentTile = Tile(id);
        } else {
            std::vector<char> row{};
            for (char c : buffer) {
                row.push_back(c);
            }
            currentTile.addRow(row);
        }
    }
    tiles.push_back(currentTile);

    inputFile.close();
    std::cout << "Total number of tiles: " << tiles.size() << std::endl;

    std::vector<Tile> cornerTiles{};
    std::vector<Tile> edgeTiles{};
    std::vector<Tile> centerTiles{};
    for (Tile& tile : tiles) {
        int matches = 0;
        for (Tile& otherTile : tiles) {
            if (otherTile.id != tile.id && tile.linesUpAny(otherTile)) {
                ++matches;
            }
        }
        switch (matches) {
        case 4:
            centerTiles.push_back(tile);
            break;
        case 3:
            edgeTiles.push_back(tile);
            break;
        case 2:
            cornerTiles.push_back(tile);
            break;

        default:
            std::cout << "Error: invalid number of matches for tile " << tile.id << ": " << matches
                      << std::endl;
            return 0;
        }
    }

    std::cout << "Number of corner tiles: " << cornerTiles.size() << std::endl;
    std::cout << "Number of edge tiles: " << edgeTiles.size() << std::endl;
    std::cout << "Number of center tiles: " << centerTiles.size() << std::endl;

    for (const Tile& tile : cornerTiles) {
        std::cout << tile.id << std::endl;
    }

    // Part 2

    int sideSize = static_cast<int>(std::sqrt(tiles.size()) + 0.1);
    std::cout << "Final image side size: " << sideSize << std::endl;
    std::vector<std::vector<Tile>> assembledTiles{};

    Tile topLeftCorner = cornerTiles.at(0);
    while (!topLeftCorner.existsThatLinesUpBottomAndRight(edgeTiles)) {
        rotateRight(topLeftCorner.pixels);
        std::cout << "Rotating top left corner..." << std::endl;
    }

    for (int i = 0; i < sideSize; ++i) {
        std::vector<Tile> row{};

        for (int j = 0; j < sideSize; ++j) {
            if (j == 0) {
                if (i == 0) {
                    row.push_back(topLeftCorner);
                } else {
                    Tile justAbove = assembledTiles.back().at(0);
                    row.push_back(justAbove.findTheOneThatLinesUpBottom(tiles));
                }
            } else {
                Tile justLeft = row.back();
                row.push_back(justLeft.findTheOneThatLinesUpRight(tiles));
            }
        }

        assembledTiles.push_back(row);
    }
    printIds(assembledTiles);

    auto image = toImage(assembledTiles);

    if (!flipUntilTrue(image, [&image]() { return countSeaMonsters(image) > 0; })) {
        rotateRight(image);
    }
    flipUntilTrue(image, [&image]() { return countSeaMonsters(image) > 0; });

    int nSeaMonsters = countSeaMonsters(image);
    std::cout << "Number of sea monsters: " << nSeaMonsters << std::endl;

    int nHashSigns = 0;
    for (const auto& row : image) {
        for (char c : row) {
            if (c == '#') {
                ++nHashSigns;
            }
        }
    }

    std::cout << "Number of hash signs: " << nHashSigns << std::endl;

    std::cout << "Number of hash signs that are not part of a sea monster: "
              << nHashSigns - nSeaMonsters * MONSTER_HASH_SIGNS_POS.size() << std::endl;

    return 0;
}