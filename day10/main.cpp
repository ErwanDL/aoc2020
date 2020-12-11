#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int product1JoltDiffsBy3JoltDiffs(const std::vector<int>& sortedAdapters) {
    std::array<int, 3> joltDifferencesCount;

    for (int i = 0; i < sortedAdapters.size() - 1; ++i) {
        int joltDiff = sortedAdapters.at(i + 1) - sortedAdapters.at(i);
        ++joltDifferencesCount.at(joltDiff - 1);
    }

    return joltDifferencesCount.at(0) * joltDifferencesCount.at(2);
}

long countArrangements(int n, const std::unordered_map<int, long>& subArrangements) {
    long res = 0;
    for (int i = 1; i <= 3; ++i) {
        if (subArrangements.count(n - i)) {
            res += subArrangements.at(n - i);
        }
    }
    return res;
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    std::vector<int> adapters{};

    adapters.push_back(0);
    while (std::getline(inputFile, buffer)) {
        adapters.push_back(std::stoi(buffer));
    }
    inputFile.close();

    // Part 1
    std::sort(adapters.begin(), adapters.end());
    adapters.push_back(adapters.back() + 3);

    std::cout << "Product of the nb of 1-jolt diffs by 3-jolt diffs: "
              << product1JoltDiffsBy3JoltDiffs(adapters) << std::endl;

    // Part 2
    std::unordered_map<int, long> subArrangements{};

    for (int adapter : adapters) {
        if (adapter == 0) {
            subArrangements.insert({0, 1});
        } else {
            long nArrangements = countArrangements(adapter, subArrangements);
            subArrangements.insert({adapter, nArrangements});
        }
    }

    std::cout << "Total number of arrangements: " << subArrangements.at(adapters.back())
              << std::endl;

    return 0;
}