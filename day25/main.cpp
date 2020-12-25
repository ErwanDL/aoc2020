#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

long transformOnce(long value, long subjectNumber) { return (subjectNumber * value) % 20201227L; }

long transform(long subjectNumber, long loopSize) {
    long value = 1L;
    for (long i = 0; i < loopSize; ++i) {
        value = transformOnce(value, subjectNumber);
    }

    return value;
}

long findLoopSize(long finalValue, long subjectNumber) {
    long loopSize = 0L;
    long value = 1L;

    while (value != finalValue) {
        value = transformOnce(value, subjectNumber);
        ++loopSize;
    }

    return loopSize;
}

int main(int argc, char* argv[]) {
    const std::string inputFileName = argv[1];
    std::ifstream inputFile{inputFileName};
    std::string buffer;

    std::getline(inputFile, buffer);
    long cardPublicKey = std::stoi(buffer);

    std::getline(inputFile, buffer);
    long doorPublicKey = std::stoi(buffer);

    inputFile.close();

    long cardLoopSize = findLoopSize(cardPublicKey, 7);
    long doorLoopSize = findLoopSize(doorPublicKey, 7);

    long encryptionKey = transform(cardPublicKey, doorLoopSize);
    assert(encryptionKey == transform(doorPublicKey, cardLoopSize) &&
           "Both encryption keys must be identical");

    std::cout << "Encryption key: " << encryptionKey << std::endl;

    return 0;
}