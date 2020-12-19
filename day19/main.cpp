#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

std::vector<std::string> split(const std::string& str, const std::string& separator) {
    std::vector<std::string> res{};
    std::size_t cursor = 0;
    std::size_t separatorPos;

    while ((separatorPos = str.find(separator, cursor)) != std::string::npos) {
        res.push_back(str.substr(cursor, separatorPos - cursor));
        cursor = separatorPos + separator.size();
    }
    res.push_back(str.substr(cursor));

    return res;
}

bool canDeriveMessage(
    std::string::iterator& msgIt, const std::string::iterator& msgEnd,
    const std::unordered_map<std::string,
                             std::variant<char, std::vector<std::vector<std::string>>>>& rules,
    const std::string& startRuleId = "0") {
    if (msgIt == msgEnd) {
        return false;
    }
    const auto& rule = rules.at(startRuleId);

    if (std::holds_alternative<char>(rule)) {
        bool charMatches = *msgIt == std::get<char>(rule);
        ++msgIt;
        return charMatches;
    } else {
        for (const std::vector<std::string>& derivation :
             std::get<std::vector<std::vector<std::string>>>(rule)) {
            auto it = msgIt;
            bool isValidDerivation = true;

            for (const std::string& subrule : derivation) {
                if (!canDeriveMessage(it, msgEnd, rules, subrule)) {
                    isValidDerivation = false;
                    break;
                }
            }
            if (isValidDerivation) {
                if (startRuleId == "0" && it != msgEnd) {
                    return false;
                }
                msgIt = it;
                return true;
            }
        }
        return false;
    }
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;

    std::unordered_map<std::string, std::variant<char, std::vector<std::vector<std::string>>>>
        rules{};

    while (std::getline(inputFile, buffer) && buffer != "") {
        auto parts = split(buffer, ": ");
        if (parts.at(1).at(0) == '"') { // rule of the form XX: "c"
            rules.insert({parts.at(0), parts.at(1).at(1)});
        } else { // rule of the form XX: YY | ZZ
            auto alternationsStr = split(parts.at(1), " | ");
            std::vector<std::vector<std::string>> alternations{};

            for (const std::string& s : alternationsStr) {
                auto derivation = split(s, " ");
                // Reversing derivations because we are going to parse from the end
                // to handle the 0: 8 11 case more easily
                std::reverse(derivation.begin(), derivation.end());
                alternations.push_back(derivation);
            }
            rules.insert({parts.at(0), alternations});
        }
    }

    // 8: 42 | 42 8
    // reversing this one doesnt change anything + we don't want left recursivity
    rules["8"] = std::vector<std::vector<std::string>>(
        {std::vector<std::string>({"42", "8"}), std::vector<std::string>({"42"})});

    // 11: 42 31 | 42 11 31
    // also reversing this one
    rules["11"] = std::vector<std::vector<std::string>>(
        {std::vector<std::string>({"31", "42"}), std::vector<std::string>({"31", "11", "42"})});

    int validMessages = 0;
    while (std::getline(inputFile, buffer)) {
        std::cout << "Message " << buffer << std::endl;
        std::reverse(buffer.begin(), buffer.end());
        auto startIt = buffer.begin();
        if (canDeriveMessage(startIt, buffer.end(), rules)) {
            std::cout << "Valid message: " << buffer << std::endl;
            ++validMessages;
        }
    }
    inputFile.close();

    std::cout << validMessages << std::endl;

    return 0;
}