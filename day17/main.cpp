#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

struct Vector4 {
    int x, y, z, w;

    Vector4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}

    bool operator==(const Vector4& o) const { return x == o.x && y == o.y && z == o.z && w == o.w; }
};

std::ostream& operator<<(std::ostream& out, const Vector4& v) {
    return out << '(' << v.x << ',' << v.y << ',' << v.z << ',' << v.w << ')';
}

struct hash_fn {
    std::size_t operator()(const Vector4& vec) const {
        std::size_t h1 = std::hash<int>()(vec.x);
        std::size_t h2 = std::hash<int>()(vec.y);
        std::size_t h3 = std::hash<int>()(vec.z);
        std::size_t h4 = std::hash<int>()(vec.w);

        return h1 ^ h2 ^ h3 ^ h4;
    }
};

int countActiveNeighbors(const Vector4& cube,
                         const std::unordered_set<Vector4, hash_fn>& activeCubes) {
    int res = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            for (int k = -1; k <= 1; ++k) {
                for (int l = -1; l <= 1; ++l) {
                    if (!(i == 0 && j == 0 && k == 0 && l == 0)) {
                        Vector4 neighbor{cube.x + i, cube.y + j, cube.z + k, cube.w + l};
                        if (activeCubes.count(neighbor)) {
                            ++res;
                        }
                    }
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

    std::unordered_set<Vector4, hash_fn> activeCubes{};

    std::pair<int, int> xRange{0, 0};
    std::pair<int, int> yRange{0, 0};
    std::pair<int, int> zRange{0, 1};
    std::pair<int, int> wRange{0, 1};

    while (std::getline(inputFile, buffer)) {
        ++xRange.second;
        yRange = {0, buffer.size()};
        for (int y = 0; y < buffer.size(); ++y) {
            if (buffer.at(y) == '#') {
                activeCubes.insert(Vector4(xRange.second - 1, y, 0, 0));
            }
        }
    }
    inputFile.close();

    for (int cycle = 1; cycle <= 6; ++cycle) {
        --xRange.first;
        ++xRange.second;
        --yRange.first;
        ++yRange.second;
        --zRange.first;
        ++zRange.second;
        --wRange.first;
        ++wRange.second;

        std::unordered_set<Vector4, hash_fn> nextActiveCubes{};
        for (int x = xRange.first; x < xRange.second; ++x) {
            for (int y = yRange.first; y < yRange.second; ++y) {
                for (int z = zRange.first; z < zRange.second; ++z) {
                    for (int w = wRange.first; w < wRange.second; ++w) {
                        Vector4 cube{x, y, z, w};
                        int nActiveNeighbors = countActiveNeighbors(cube, activeCubes);
                        if (activeCubes.count(cube) &&
                            (nActiveNeighbors == 2 || nActiveNeighbors == 3)) {
                            nextActiveCubes.insert(cube);
                        } else if (!activeCubes.count(cube) && nActiveNeighbors == 3) {
                            nextActiveCubes.insert(cube);
                        }
                    }
                }
            }
        }
        activeCubes = nextActiveCubes;

        std::cout << "Number of cubes active after " << cycle << " cycles: " << activeCubes.size()
                  << std::endl;
    }

    return 0;
}