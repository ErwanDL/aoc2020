#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

/* Part 1
int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    std::getline(inputFile, buffer);
    int departure = std::stoi(buffer);

    std::vector<std::string> inputs{};
    std::getline(inputFile, buffer);
    inputFile.close();

    boost::split(inputs, buffer, boost::is_any_of(","));
    std::vector<int> busIds{};
    for (const auto& input : inputs) {
        if (input != "x") {
            busIds.push_back(std::stoi(input));
        }
    }

    int bestBus;
    int shortestWait = std::numeric_limits<int>::max();
    for (int id : busIds) {
        if (departure % id == 0) {
            bestBus = id;
            shortestWait = 0;
            break;
        } else {
            int waitTime = id - (departure % id);
            if (waitTime < shortestWait) {
                bestBus = id;
                shortestWait = waitTime;
            }
        }
    }

    std::cout << "Best bus: " << bestBus << " with a waiting time of " << shortestWait
              << " minutes\nProduct: " << bestBus * shortestWait << std::endl;

    return 0;
}
*/

// Part 2
int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    std::getline(inputFile, buffer);
    int departure = std::stoi(buffer);

    std::vector<std::string> inputs{};
    std::getline(inputFile, buffer);
    inputFile.close();

    boost::split(inputs, buffer, boost::is_any_of(","));
    std::vector<std::pair<int, int>> busesWithOffsets{};
    for (int i = 0; i < inputs.size(); ++i) {
        if (inputs.at(i) != "x") {
            busesWithOffsets.push_back({std::stoi(inputs.at(i)), i});
        }
    }

    long currentMatch = 0;
    long currentLcm = busesWithOffsets.at(0).first;
    for (int i = 1; i < busesWithOffsets.size(); ++i) {
        long newMatch = currentMatch;
        auto [id, offset] = busesWithOffsets.at(i);
        while ((id - newMatch % id) != (offset % id)) {
            newMatch += currentLcm;
        }
        currentMatch = newMatch;
        currentLcm = std::lcm(currentLcm, id);
    }

    std::cout << "Earliest matching timestamp: " << currentMatch << std::endl;

    return 0;
}