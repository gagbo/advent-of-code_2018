#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "fuel.hpp"

void find_square_one_size(const int& grid_serial);
void find_square(const int& grid_serial);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "We need exactly two arguments.\n";
        return 1;
    }

    if (strncmp(argv[1], "a", 1) == 0) {
        find_square_one_size(std::stoi(argv[2]));
        return 0;
    }
    if (strncmp(argv[1], "b", 1) == 0) {
        find_square(std::stoi(argv[2]));
        return 0;
    }

    return 1;
}

void find_square_one_size(const int& grid_serial) {
    FuelGrid grid(grid_serial);
    grid.best_square_one_size(3);
}
void find_square(const int& grid_serial) {
    FuelGrid grid(grid_serial);
    grid.best_square();
}
