#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

bool isSumOfPreviousNumbers(const std::vector<long>& inputs, int index, int preambleSize) {
    long target = inputs.at(index);
    for (int i = index - preambleSize; i < index; ++i) {
        long a = inputs.at(i);
        for (int j = i + 1; j < index; ++j) {
            long b = inputs.at(j);
            if (a + b == target) {
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    std::vector<long> inputs{};

    constexpr int preambleSize = 25;

    while (std::getline(inputFile, buffer)) {
        inputs.push_back(std::stol(buffer));
    }

    inputFile.close();

    long firstErrorIndex;

    for (int i = preambleSize; i < inputs.size(); ++i) {
        if (!isSumOfPreviousNumbers(inputs, i, preambleSize)) {
            firstErrorIndex = i;
            std::cout << "The first number that isn't the sum of 2 previous numbers is: "
                      << inputs.at(firstErrorIndex) << std::endl;
            break;
        }
    }

    long errorValue = inputs.at(firstErrorIndex);
    int contiguousSetStart, contiguousSetEnd;
    bool found = false;

    for (int i = 0; i < firstErrorIndex; ++i) {
        int sum = inputs.at(i);
        for (int j = i + 1; j < firstErrorIndex; ++j) {
            sum += inputs.at(j);
            if (sum == errorValue) {
                contiguousSetStart = i;
                contiguousSetEnd = j;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }

    std::cout << "The contiguous set that sums to " << errorValue << " starts at "
              << contiguousSetStart << " and ends at " << contiguousSetEnd << std::endl;

    long min = *std::min_element(inputs.cbegin() + contiguousSetStart,
                                 inputs.cbegin() + contiguousSetEnd + 1);
    long max = *std::max_element(inputs.cbegin() + contiguousSetStart,
                                 inputs.cbegin() + contiguousSetEnd + 1);

    std::cout << "Min element of the contiguous set: " << min << std::endl;
    std::cout << "Max element of the contiguous set: " << max << std::endl;
    std::cout << "Sum of min and max: " << min + max << std::endl;

    return 0;
}