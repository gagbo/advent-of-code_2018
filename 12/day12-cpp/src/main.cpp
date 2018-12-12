#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "life.hpp"

static const int PART_B_THRES = 2000;
void part_a(const char* filename, long int max_turn);
void part_b(const char* filename, long int max_turn);

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
        part_b(argv[2], 50000000000);
        return 0;
    }

    return 1;
}

void part_a(const char* filename, long int max_turn) {
    std::ifstream input_stream(filename);
    std::string line;
    std::getline(input_stream, line);  // line now has the initial state to parse
    // Store the State as a LifeLine
    LifeLine game(line);
    std::getline(input_stream, line);  // line now is blank

    // There should be 32 lines left
    while ( std::getline(input_stream, line)) {
      // Store the pattern as a 8bit int from 0 to 31
      game.rules.add_pattern_from_input(line);
    }
    game.print();

    while (game.turn < max_turn) {
      auto old_sum = game.sum_pots();
      game.advance();
      game.print();
      std::cout << "Difference : " << game.sum_pots() - old_sum << "\n";
    }
    std::cout << "Result : " << game.sum_pots() << "\n";

}

void part_b(const char* filename, long int max_turn) {
    std::ifstream input_stream(filename);
    std::string line;
    std::getline(input_stream, line);  // line now has the initial state to parse
    // Store the State as a LifeLine
    LifeLine game(line);
    std::getline(input_stream, line);  // line now is blank

    // There should be 32 lines left
    while ( std::getline(input_stream, line)) {
      // Store the pattern as a 8bit int from 0 to 31
      game.rules.add_pattern_from_input(line);
    }
    game.print();

    auto difference = game.sum_pots();
    while (game.turn < PART_B_THRES) {
      auto old_sum = game.sum_pots();
      game.advance();
      game.print();
      difference = game.sum_pots() - old_sum;
      std::cout << "Difference : " << difference << "\n";
    }
    std::cout << "Result : " << game.sum_pots() + (max_turn - PART_B_THRES) * difference << "\n";

}
