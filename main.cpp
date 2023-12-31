#include <iostream>

#include "solitaire.h"

int main(int argc, char* argv[]) {
    int seed = 0;
    if (argc > 1) {
        seed = std::stoi(argv[1]);
    }

    Solitaire solitaire(seed, "output_" + std::to_string(seed) + ".txt");
    solitaire.solve();

    return 0;
}