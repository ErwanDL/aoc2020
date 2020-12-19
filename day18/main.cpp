#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

long parseOperand(std::string::reverse_iterator& cursor);

long evaluate(std::string::reverse_iterator rbegin, std::string::reverse_iterator rend) {
    auto cursor = rbegin;
    long rightOperand = parseOperand(cursor);

    ++cursor;

    if (cursor == rend) {
        return rightOperand;
    }

    if (*cursor == '+') {
        while (*cursor != '*') {
            long leftOperand = parseOperand(++cursor);
            rightOperand += leftOperand;
            ++cursor;
            if (cursor == rend) {
                return rightOperand;
            }
        }
    }
    if (*cursor == '*') {
        return evaluate(++cursor, rend) * rightOperand;
    } else {
        std::cout << "Error parsing: " << *cursor << std::endl;
        exit(0);
    }
}

long parseOperand(std::string::reverse_iterator& cursor) {
    auto rbegin = cursor;
    if (*cursor == ')') { // we're iterating in reverse, so parentheses open with ')'
        long openParentheses = 1L;
        while (openParentheses > 0) {
            ++cursor;
            if (*cursor == ')') {
                ++openParentheses;
            } else if (*cursor == '(') {
                --openParentheses;
            }
        }
        return evaluate(rbegin + 1, cursor);
    } else {
        return static_cast<long>(*cursor - '0');
    }
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;

    long sum = 0;
    while (std::getline(inputFile, buffer)) {
        buffer.erase(std::remove(buffer.begin(), buffer.end(), ' '), buffer.end());
        sum += evaluate(buffer.rbegin(), buffer.rend());
    }
    inputFile.close();

    std::cout << sum << std::endl;

    return 0;
}