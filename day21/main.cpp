#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
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

std::unordered_set<std::string> setIntersection(const std::unordered_set<std::string>& set1,
                                                const std::unordered_set<std::string>& set2) {
    std::unordered_set<std::string> res{};
    for (const auto& s : set1) {
        if (set2.count(s)) {
            res.insert(s);
        }
    }
    return res;
}
std::string findNewUnambiguousAllergen(
    const std::unordered_map<std::string, std::unordered_set<std::string>>& allergen2possIngr,
    const std::unordered_set<std::string>& foundUnambiguousAllergens) {
    for (const auto& [key, val] : allergen2possIngr) {
        if (val.size() == 1 && !foundUnambiguousAllergens.count(key)) {
            return key;
        }
    }
    throw std::runtime_error("No unambiguous allergen found");
}

bool allSizeOne(
    const std::unordered_map<std::string, std::unordered_set<std::string>>& allergen2possIngr) {
    for (const auto& [key, val] : allergen2possIngr) {
        if (val.size() > 1) {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    const std::regex foodRegex{"([a-z ]*) \\(contains ([a-z ,]*)\\)"};
    std::unordered_map<std::string, std::unordered_set<std::string>> allergen2possIngr{};
    std::vector<std::string> allIngredients{};

    while (std::getline(inputFile, buffer)) {
        std::smatch matches;
        std::regex_match(buffer, matches, foodRegex);
        std::vector<std::string> ingredients = split(matches[1].str(), " ");
        allIngredients.insert(allIngredients.end(), ingredients.begin(), ingredients.end());
        std::vector<std::string> allergens = split(matches[2].str(), ", ");
        for (const auto& allergen : allergens) {
            std::unordered_set<std::string> ingrSet{ingredients.begin(), ingredients.end()};
            if (allergen2possIngr.count(allergen)) {
                allergen2possIngr[allergen] = setIntersection(allergen2possIngr[allergen], ingrSet);
            } else {
                allergen2possIngr[allergen] = ingrSet;
            }
        }
    }
    inputFile.close();

    std::unordered_set<std::string> foundUnambiguousAllergens{};
    while (!allSizeOne(allergen2possIngr)) {
        std::string unambAllergen =
            findNewUnambiguousAllergen(allergen2possIngr, foundUnambiguousAllergens);
        foundUnambiguousAllergens.insert(unambAllergen);
        std::string correspondingIngr = *(allergen2possIngr[unambAllergen].begin());

        for (auto& [key, val] : allergen2possIngr) {
            if (key != unambAllergen) {
                val.erase(correspondingIngr);
            }
        }
    }

    std::unordered_set<std::string> ingrWithAllergen{};
    for (const auto& [key, val] : allergen2possIngr) {
        ingrWithAllergen.insert(*(val.begin()));
    }

    int nNonAllergenicIngredients = 0;
    for (const std::string& ingr : allIngredients) {
        if (!ingrWithAllergen.count(ingr)) {
            ++nNonAllergenicIngredients;
        }
    }

    std::cout << nNonAllergenicIngredients << std::endl;

    // Part 2

    std::vector<std::string> allergens{};
    for (const auto& [key, val] : allergen2possIngr) {
        allergens.push_back(key);
    }
    std::sort(allergens.begin(), allergens.end());
    for (const std::string& allerg : allergens) {
        char sep = allerg == allergens.back() ? '\n' : ',';
        std::cout << *(allergen2possIngr[allerg].begin()) << sep;
    }

    return 0;
}