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
    std::vector<int> inputs{};
    while (std::getline(inputFile, buffer)) {
        inputs.push_back(std::stoi(buffer));
    }
    inputFile.close();

    /*
     * Use the `inputs` to solve the problem !
     */

    return 0;
}