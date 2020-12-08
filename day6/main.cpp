#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::unordered_set<char> initSetWithAllLetters() {
    std::unordered_set<char> set{};
    for (char c = 'a'; c <= 'z'; ++c) {
        set.insert(c);
    }
    return set;
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    std::vector<std::unordered_set<char>> groups{initSetWithAllLetters()};
    while (std::getline(inputFile, buffer)) {
        if (buffer == "") {
            groups.push_back(initSetWithAllLetters());
        } else {
            std::unordered_set<char> intersection{};
            const auto& lastGroup = groups.back();
            for (char c : buffer) {
                if (lastGroup.count(c)) {
                    intersection.insert(c);
                }
            }
            groups.back() = intersection;
        }
    }
    inputFile.close();

    int sumQuestions =
        std::accumulate(groups.cbegin(), groups.cend(), 0,
                        [](int res, const std::unordered_set<char>& g) { return res + g.size(); });

    std::cout << "Total number of questions answered by 'yes': " << sumQuestions << std::endl;

    return 0;
}