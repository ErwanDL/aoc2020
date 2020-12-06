#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    std::vector<int> inputs{};
    while (std::getline(inputFile, buffer)) {
        inputs.push_back(std::stoi(buffer));
    }
    inputFile.close();

    for (int i = 0; i < inputs.size() - 2; ++i) {
        int entry1 = inputs.at(i);
        for (int j = i + 1; j < inputs.size() - 1; ++j) {
            int entry2 = inputs.at(j);
            for (int k = j + 1; k < inputs.size(); ++k) {
                int entry3 = inputs.at(k);
                if (entry1 + entry2 + entry3 == 2020) {
                    std::cout << entry1 * entry2 * entry3 << std::endl;
                    return 0;
                }
            }
        }
    }

    std::cout << "Error: entries summing to 2020 not found" << std::endl;
    return 0;
}