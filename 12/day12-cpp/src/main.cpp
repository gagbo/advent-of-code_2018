#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "life.hpp"

void part_a(const char* filename, int max_turn);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "We need exactly two arguments.\n";
        return 1;
    }

    if (strncmp(argv[1], "a", 1) == 0) {
        part_a(argv[2], 20);
        return 0;
    }
    if (strncmp(argv[1], "b", 1) == 0) {
        part_a(argv[2], 50000000000);
        return 0;
    }

    return 1;
}

void part_a(const char* filename, int max_turn) {
    std::ifstream input_stream(filename);
    std::string line;
    std::getline(input_stream, line);  // line now has the initial state to parse
    // Store the State as a LifeLine
    LifeLine game(line);
    std::getline(input_stream, line);  // line now is blank

    // There should be 32 lines left
    while ( std::getline(input_stream, line)) {
      // TODO : check that the line is not blank
      // Store the pattern as a 8bit int from 0 to 31
      game.rules.add_pattern_from_input(line);
    }
    game.print();

    while (game.turn < max_turn) {
      game.advance();
      game.print();
    }
    std::cout << "Result : " << game.sum_pots() << "\n";

}
