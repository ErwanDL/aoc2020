#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Part 1
std::pair<unsigned long, unsigned long> parseMask1(const std::string& stringMask) {
    unsigned long zerosMask = 0;
    unsigned long onesMask = 0;

    for (char c : stringMask) {
        zerosMask <<= 1;
        onesMask <<= 1;
        if (c == 'X') {
            zerosMask |= 1;
        } else if (c == '1') {
            zerosMask |= 1;
            onesMask |= 1;
        }
    }

    return {zerosMask, onesMask};
}

// Part 2
std::pair<unsigned long, std::vector<int>> parseMask2(const std::string& stringMask) {
    unsigned long onesMask = 0UL;
    std::vector<int> floatingBits{};

    for (int i = 0; i < stringMask.size(); ++i) {
        onesMask <<= 1UL;
        if (stringMask.at(i) == '1') {
            onesMask |= 1UL;
        } else if (stringMask.at(i) == 'X') {
            floatingBits.push_back(stringMask.size() - i - 1);
        }
    }

    return {onesMask, floatingBits};
}

std::pair<unsigned long, unsigned long> floatAddresses(unsigned long baseAddress,
                                                       int floatingBitIdx) {
    unsigned long bit = 1UL << floatingBitIdx;
    return {baseAddress | bit, baseAddress & ~bit};
}

void insertFloatingAddresses(unsigned long baseAddress, unsigned long value,
                             std::unordered_map<unsigned long, unsigned long>& memory,
                             std::vector<int> floatingBits) {
    if (floatingBits.size() == 0) {
        memory[baseAddress] = value;
    } else {
        int floatingBitIdx = floatingBits.back();
        floatingBits.pop_back();

        auto [firstAddress, secondAddress] = floatAddresses(baseAddress, floatingBitIdx);
        insertFloatingAddresses(firstAddress, value, memory, floatingBits);
        insertFloatingAddresses(secondAddress, value, memory, floatingBits);
    }
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    std::unordered_map<unsigned long, unsigned long> memory{};
    unsigned long onesMask;
    std::vector<int> floatingBits;
    const std::regex assignmentRegex{"mem\\[([0-9]+)\\] = ([0-9]+)"};

    while (std::getline(inputFile, buffer)) {
        if (buffer.substr(0, 7) == "mask = ") {
            std::string stringMask = buffer.substr(7);
            auto masks = parseMask2(stringMask);
            onesMask = masks.first;
            floatingBits = masks.second;
        } else {
            std::smatch matches;
            std::regex_match(buffer, matches, assignmentRegex);
            unsigned long address = std::stoul(matches[1].str());
            unsigned long value = std::stoul(matches[2].str());

            address |= onesMask;

            insertFloatingAddresses(address, value, memory, floatingBits);
        }
    }
    inputFile.close();

    unsigned long sum = 0;
    for (auto& [address, value] : memory) {
        sum += value;
    }

    std::cout << "Sum of all values left in memory: " << sum << std::endl;

    return 0;
}