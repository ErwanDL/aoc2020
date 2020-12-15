#include <array>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <chrono>
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
    std::vector<std::string> inputs{};
    constexpr int N_TURNS = 30'000'000;

    // std::unordered_map<int, int> number2lastSpoken{};
    //
    // Using a vector instead of a map incurs a significant increase in execution
    // speed, from ~17s to ~1s on my machine for the second part of the problem.
    std::vector<int> number2lastSpoken(N_TURNS, 0);

    std::getline(inputFile, buffer);
    inputFile.close();

    boost::split(inputs, buffer, boost::is_any_of(","));

    for (int i = 0; i < inputs.size() - 1; ++i) {
        int number = std::stoi(inputs.at(i));
        number2lastSpoken[number] = i + 1;
    }

    int lastTurn = inputs.size();
    int lastNumber = std::stoi(inputs.back());

    auto start = std::chrono::high_resolution_clock::now();
    while (lastTurn < N_TURNS) {
        int numberToSay;
        if (!number2lastSpoken[lastNumber]) {
            numberToSay = 0;
        } else {
            numberToSay = lastTurn - number2lastSpoken[lastNumber];
        }
        number2lastSpoken[lastNumber] = lastTurn;
        lastNumber = numberToSay;
        ++lastTurn;
    }

    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << N_TURNS << "th number spoken: " << lastNumber << std::endl;
    std::cout << "Time elapsed: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms"
              << std::endl;

    return 0;
}