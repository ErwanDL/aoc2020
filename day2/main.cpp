#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;

    const std::regex re{"([0-9]+)-([0-9]+) ([a-z]): ([a-z]+)"};

    int validPasswordsCount = 0;

    while (std::getline(inputFile, buffer)) {
        std::smatch matches;
        std::regex_match(buffer, matches, re);
        const int posA = std::stoi(matches[1].str());
        const int posB = std::stoi(matches[2].str());
        const char letter = matches[3].str()[0];
        const std::string word = matches[4].str();

        const bool atPosA = word[posA - 1] == letter;
        const bool atPosB = word[posB - 1] == letter;

        if (atPosA != atPosB) {
            ++validPasswordsCount;
        }
    }

    std::cout << validPasswordsCount << std::endl;

    inputFile.close();
    return 0;
}