#include <array>
#include <iostream>
#include <Random.h>

int const NUMS = 20;
int const NUM_RUNS = 10'000'000;

int main() {
    std::array<int, NUMS> amountOfTimesPicked {};

    Random rand;

    std::cout << "Starting...\n";

    for (int i = 0; i < NUM_RUNS; ++i) {
        int pickedIndex = 0;

        for (int candidateIndex = 0; candidateIndex < NUMS; ++candidateIndex) {
            if (rand.range(candidateIndex + 1) == 0) {
                pickedIndex = candidateIndex;
            }
        }

        ++amountOfTimesPicked[pickedIndex];
    }

    std::cout << "Finished.\n";

    double avg = 0;
    for (auto x : amountOfTimesPicked) {
        std::cout << x << "\n";
        avg += x / NUMS;
    }

    std::cout << "Avg: " << avg << "\n";


    return 0;
}
