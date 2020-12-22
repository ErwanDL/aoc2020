#include <algorithm>
#include <deque>
#include <exception>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using Deck = std::deque<int>;

void printDeck(const Deck& deck) {
    for (int card : deck) {
        std::cout << card << ' ';
    }
    std::cout << std::endl;
}

Deck pickNNextCards(const Deck& deck, int n) {
    Deck res{};
    for (auto it = deck.begin(); it != deck.begin() + n; ++it) {
        res.push_back(*it);
    }

    return res;
}

struct DeckHash {
    size_t operator()(const Deck& v) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct DeckPairHash {
    size_t operator()(const std::pair<Deck, Deck>& v) const {
        DeckHash hasher{};
        return hasher(v.first) ^ hasher(v.second);
    }
};

int calculateScore(const Deck& deck) {
    int score = 0;
    int size = deck.size();
    for (int i = 0; i < size; ++i) {
        score += (size - i) * deck.at(i);
    }
    return score;
}

int playRecursiveCombatGame(Deck& p1Deck, Deck& p2Deck) {
    static int nGames = 0;
    ++nGames;
    if (nGames % 1000 == 0) {
        std::cout << "GAME " << nGames << std::endl;
    }

    std::unordered_set<std::pair<Deck, Deck>, DeckPairHash> playedDecks{};
    // std::vector<long> seenGameStates{};

    while (!(p1Deck.empty() || p2Deck.empty())) {
        std::pair<Deck, Deck> currentSituation{p1Deck, p2Deck};
        if (playedDecks.count(currentSituation)) {
            return 1;
        }
        playedDecks.insert(currentSituation);

        // An interesting optimization heuristic I found on the Reddit megathread after
        // having finished my solution : it seems that detecting loops in the scores
        // (although it does not guarantee the decks are exactly identical) has the same
        // effect, but it is much faster.
        // Here, using a vector is as fast as using an unordered_set (which is definitely
        // not the case when storing the full decks).
        //
        // long gameState = calculateScore(p1Deck) * calculateScore(p2Deck);
        // if (std::find(seenGameStates.begin(), seenGameStates.end(), gameState) !=
        //     seenGameStates.end()) {
        //     return 1;
        // }
        // seenGameStates.push_back(gameState);

        int card1 = p1Deck.front();
        p1Deck.pop_front();
        int card2 = p2Deck.front();
        p2Deck.pop_front();

        int roundWinner;

        if (p1Deck.size() >= card1 && p2Deck.size() >= card2) {
            Deck p1NextCards = pickNNextCards(p1Deck, card1);
            Deck p2NextCards = pickNNextCards(p2Deck, card2);
            roundWinner = playRecursiveCombatGame(p1NextCards, p2NextCards);
        } else {
            roundWinner = card1 > card2 ? 1 : 2;
        }

        if (roundWinner == 1) {
            p1Deck.push_back(card1);
            p1Deck.push_back(card2);
        } else {
            p2Deck.push_back(card2);
            p2Deck.push_back(card1);
        }
    }

    return p1Deck.empty() ? 2 : 1;
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;
    Deck p1Deck{};
    Deck p2Deck{};

    std::getline(inputFile, buffer);
    while (std::getline(inputFile, buffer) && buffer != "") {
        p1Deck.push_back(std::stoi(buffer));
    }

    std::getline(inputFile, buffer);
    while (std::getline(inputFile, buffer)) {
        p2Deck.push_back(std::stoi(buffer));
    }

    inputFile.close();

    int winner = playRecursiveCombatGame(p1Deck, p2Deck);
    Deck winningDeck = winner == 1 ? p1Deck : p2Deck;

    int score = calculateScore(winningDeck);

    std::cout << "Winner: " << winner << "\nWinning deck score: " << score << std::endl;

    return 0;
}