#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

void printProgram(const std::vector<std::pair<std::string, int>>& program) {
    std::cout << "Program: [";
    for (const auto& instr : program) {
        std::cout << instr.first << ' ' << instr.second << ", ";
    }
    std::cout << "]" << std::endl;
}

std::pair<bool, int> terminates(const std::vector<std::pair<std::string, int>>& program) {
    std::unordered_set<int> executedInstructionLines{};
    int acc = 0;
    int i = 0;
    while (true) {
        const std::string& instr = program.at(i).first;
        int value = program.at(i).second;

        executedInstructionLines.insert(i);
        if (instr == "nop") {
            ++i;
        } else if (instr == "acc") {
            acc += value;
            ++i;
        } else if (instr == "jmp") {
            i += value;
        }

        if (executedInstructionLines.count(i)) {
            return {false, acc};
        } else if (i == program.size()) {
            return {true, acc};
        }
    }
}

void swapJmpNop(std::pair<std::string, int>& line) {
    if (line.first == "nop") {
        line.first = "jmp";
    } else if (line.first == "jmp") {
        line.first = "nop";
    }
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    std::vector<std::pair<std::string, int>> program{};
    while (std::getline(inputFile, buffer)) {
        program.push_back({buffer.substr(0, 3), std::stoi(buffer.substr(4))});
    }
    inputFile.close();

    for (auto& line : program) {
        swapJmpNop(line);
        auto [terminated, accValue] = terminates(program);
        if (terminated) {
            std::cout << "Value in acc when program terminates: " << accValue << std::endl;
        }
        swapJmpNop(line);
    }

    return 0;
}