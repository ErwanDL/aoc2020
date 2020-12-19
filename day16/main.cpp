#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <class T> void printVector(const std::vector<T>& vec) {
    std::cout << "{ ";
    for (std::size_t i = 0; i < vec.size() - 1; ++i) {
        std::cout << vec.at(i) << ", ";
    }
    std::cout << vec.back() << " }" << std::endl;
}

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

bool allSizeOne(const std::vector<std::unordered_set<std::string>>& possibleFieldsByPos) {
    for (const auto& fields : possibleFieldsByPos) {
        if (fields.size() > 1) {
            return false;
        }
    }
    return true;
}

class Ranges {
    int low1, high1, low2, high2;

    Ranges(int low1, int high1, int low2, int high2)
        : low1(low1), high1(high1), low2(low2), high2(high2) {}

  public:
    bool contains(int value) const {
        return (low1 <= value && value <= high1) || (low2 <= value && value <= high2);
    }

    static Ranges fromString(const std::string& rangesStr) {
        auto rangesVec = split(rangesStr, " or ");
        auto range1 = split(rangesVec.at(0), "-");
        auto range2 = split(rangesVec.at(1), "-");

        return Ranges(std::stoi(range1.at(0)), std::stoi(range1.at(1)), std::stoi(range2.at(0)),
                      std::stoi(range2.at(1)));
    }
};

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    std::unordered_map<std::string, Ranges> rules{};
    std::vector<int> yourTicket{};
    std::vector<std::vector<int>> nearbyTickets{};

    while (std::getline(inputFile, buffer) && buffer != "") {
        auto splitRule = split(buffer, ": ");
        rules.insert({splitRule.at(0), Ranges::fromString(splitRule.at(1))});
    }

    std::getline(inputFile, buffer); // "your ticket:"
    std::getline(inputFile, buffer);

    for (const std::string& val : split(buffer, ",")) {
        yourTicket.push_back(std::stoi(val));
    }

    std::getline(inputFile, buffer);
    std::getline(inputFile, buffer); // "nearby tickets:"

    while (std::getline(inputFile, buffer)) {
        std::vector<int> ticket{};
        for (const std::string& val : split(buffer, ",")) {
            ticket.push_back(std::stoi(val));
        }
        nearbyTickets.push_back(ticket);
    }

    inputFile.close();

    int errorRate = 0;
    std::vector<std::vector<int>> validNearbyTickets{};

    for (const auto& ticket : nearbyTickets) {
        bool isValid = true;
        for (int val : ticket) {
            bool matchesRule = false;
            for (const auto& [ruleName, ranges] : rules) {
                if (ranges.contains(val)) {
                    matchesRule = true;
                    break;
                }
            }
            if (!matchesRule) {
                isValid = false;
                errorRate += val;
            }
        }
        if (isValid) {
            validNearbyTickets.push_back(ticket);
        }
    }

    std::cout << "Ticket scanning error rate: " << errorRate << std::endl;

    int nFields = yourTicket.size();

    std::vector<std::unordered_set<std::string>> possibleFieldsByPos{};

    // Building initial values for the possible fields using the narrator's ticket
    for (int i = 0; i < nFields; ++i) {
        possibleFieldsByPos.push_back({});
        for (const auto& [ruleName, ranges] : rules) {
            if (ranges.contains(yourTicket.at(i))) {
                possibleFieldsByPos.at(i).insert(ruleName);
            }
        }
    }

    for (const auto& ticket : validNearbyTickets) {
        for (int i = 0; i < nFields; ++i) {
            for (const auto& [ruleName, ranges] : rules) {
                if (!ranges.contains(ticket.at(i))) {
                    possibleFieldsByPos.at(i).erase(ruleName);
                }
            }
        }
    }

    while (!allSizeOne(possibleFieldsByPos)) {
        for (const auto& possibleFields : possibleFieldsByPos) {
            if (possibleFields.size() == 1) {
                std::string onlyViableField = *possibleFields.begin();
                for (auto& otherPossibleFields : possibleFieldsByPos) {
                    if (&otherPossibleFields != &possibleFields) {
                        otherPossibleFields.erase(onlyViableField);
                    }
                }
            }
        }
    }

    long product = 1;
    for (int i = 0; i < nFields; ++i) {
        const std::string fieldName = *possibleFieldsByPos.at(i).begin();
        std::cout << "Field name at position " << i << ": " << fieldName << std::endl;
        if (fieldName.substr(0, 9) == "departure") {
            product *= yourTicket.at(i);
        }
    }

    std::cout << "Product of 'departure' fields: " << product << std::endl;

    return 0;
}