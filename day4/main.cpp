#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

bool validateKeyVals(std::string key, std::string val) {
    if (key == "byr") {
        std::regex re{"[0-9]{4}"};
        std::smatch matches;
        if (std::regex_match(val, matches, re)) {
            int year = std::stoi(val);
            if (year >= 1920 && year <= 2002) {
                return true;
            }
        }
    } else if (key == "iyr") {
        std::regex re{"[0-9]{4}"};
        std::smatch matches;
        if (std::regex_match(val, matches, re)) {
            int year = std::stoi(val);
            if (year >= 2010 && year <= 2020) {
                return true;
            }
        }
    } else if (key == "eyr") {
        std::regex re{"[0-9]{4}"};
        std::smatch matches;
        if (std::regex_match(val, matches, re)) {
            int year = std::stoi(val);
            if (year >= 2020 && year <= 2030) {
                return true;
            }
        }
    } else if (key == "hgt") {
        std::regex re{"[0-9]+(cm|in)"};
        std::smatch matches;
        if (std::regex_match(val, matches, re)) {
            std::string unit = val.substr(val.length() - 2);
            int amount = std::stoi(val.substr(0, val.length() - 2));
            if ((unit == "cm" && amount >= 150 && amount <= 193) ||
                (unit == "in" && amount >= 59 && amount <= 76)) {
                return true;
            }
        }
    } else if (key == "hcl") {
        std::regex re{"#[0-9a-f]{6}"};
        std::smatch matches;
        return std::regex_match(val, matches, re);
    } else if (key == "ecl") {
        std::unordered_set<std::string> possibleValues{
            {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"}};
        return possibleValues.count(val) > 0;
    } else if (key == "pid") {
        std::regex re{"[0-9]{9}"};
        std::smatch matches;
        return std::regex_match(val, matches, re);
    }
    return false;
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;

    std::vector<std::string> passports;
    std::string currentPassport;
    while (std::getline(inputFile, buffer)) {
        if (buffer == "") {
            passports.push_back(currentPassport);
            currentPassport = "";
        } else {
            currentPassport += " ";
            currentPassport += buffer;
        }
    }
    passports.push_back(currentPassport);

    int validPassports = 0;
    const std::unordered_set<std::string> expectedKeys{
        {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"}};

    for (auto& s : passports) {
        std::vector<std::string> keyValPairs{};
        boost::split(keyValPairs, s, boost::is_any_of(" "));
        std::unordered_set<std::string> presentKeys{};
        for (auto& keyVal : keyValPairs) {
            std::string key = keyVal.substr(0, 3);
            if (keyVal.length() > 4) {
                std::string val = keyVal.substr(4);
                if (validateKeyVals(key, val) && expectedKeys.count(key)) {
                    presentKeys.insert(key);
                }
            }
        }

        if (presentKeys.size() == expectedKeys.size()) {
            std::cout << "Valid " << s << std::endl;
            ++validPassports;
        } else {
            std::cout << "Invalid " << s << std::endl;
        }
    }

    std::cout << validPassports << std::endl;

    return 0;
}