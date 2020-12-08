#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

void parseAndAddToHierarchy(
    std::unordered_map<std::string, std::vector<std::pair<int, std::string>>>&
        parentToChildrenWithCount,
    const std::string& line) {
    std::regex ruleRegex{"([a-z]+ [a-z]+) bags contain(.*)"};
    std::smatch matches;
    std::regex_match(line, matches, ruleRegex);
    std::string parentBag = matches[1].str();

    if (matches[2].str() == " no other bags.") {
        parentToChildrenWithCount[parentBag] = {};
        return;
    }

    std::vector<std::string> childrenBags{};
    std::vector<std::pair<int, std::string>> parsedChildrenBags{};
    boost::split(childrenBags, matches[2].str(), boost::is_any_of(","));

    std::regex childBagRegex{" ([1-9]) ([a-z]+ [a-z]+) bags?\\.?"};

    for (const auto& childBag : childrenBags) {
        std::smatch childMatches;
        std::regex_match(childBag, childMatches, childBagRegex);
        parsedChildrenBags.push_back({std::stoi(childMatches[1].str()), childMatches[2].str()});
    }

    parentToChildrenWithCount[parentBag] = parsedChildrenBags;
}

std::unordered_map<std::string, std::vector<std::string>>
invertHierarchy(const std::unordered_map<std::string, std::vector<std::pair<int, std::string>>>&
                    parentToChildrenWithCount) {
    std::unordered_map<std::string, std::vector<std::string>> childToParents{};
    for (const auto& [parent, children] : parentToChildrenWithCount) {
        childToParents.insert({parent, std::vector<std::string>()});
    }

    for (const auto& [parent, children] : parentToChildrenWithCount) {
        for (const auto& [count, childBag] : children) {
            childToParents[childBag].push_back(parent);
        }
    }
    return childToParents;
}

void buildAllParents(
    const std::string& child, std::unordered_set<std::string>& parentsSet,
    const std::unordered_map<std::string, std::vector<std::string>>& childToParents) {
    for (const auto& parent : childToParents.at(child)) {
        parentsSet.insert(parent);
        buildAllParents(parent, parentsSet, childToParents);
    }
}

int nIncludedBags(const std::string& parent,
                  const std::unordered_map<std::string, std::vector<std::pair<int, std::string>>>&
                      parentToChildrenWithCount) {
    const auto& children = parentToChildrenWithCount.at(parent);
    return 1 + std::accumulate(children.cbegin(), children.cend(), 0,
                               [&parentToChildrenWithCount](
                                   int res, const std::pair<int, std::string>& childWithCount) {
                                   return res + childWithCount.first *
                                                    nIncludedBags(childWithCount.second,
                                                                  parentToChildrenWithCount);
                               });
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    // Map of each bag type to the list of children bags it contains
    std::unordered_map<std::string, std::vector<std::pair<int, std::string>>>
        parentToChildrenWithCount{};

    while (std::getline(inputFile, buffer)) {
        parseAndAddToHierarchy(parentToChildrenWithCount, buffer);
    }

    inputFile.close();

    const auto childToParents = invertHierarchy(parentToChildrenWithCount);

    std::unordered_set<std::string> shinyGoldAllParents{};
    buildAllParents("shiny gold", shinyGoldAllParents, childToParents);

    std::cout << "Total number of parents of 'shiny gold': " << shinyGoldAllParents.size()
              << std::endl;

    std::cout << "A 'shiny gold' bag must contain "
              << nIncludedBags("shiny gold", parentToChildrenWithCount) - 1 << " other bags."
              << std::endl;

    return 0;
}