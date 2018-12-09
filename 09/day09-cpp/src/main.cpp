#include <fstream>
#include <iostream>
#include <string>

#include "game.h"

int pass_tests();
int test(std::string filename, unsigned long long expected);
unsigned long long int run_game(const char* filename);

int main(int argc, char* argv[]) {
    if (argc == 1) {
      std::cout << "No argument given : going for tests. Specify path to input file to run a test case\n";
      return pass_tests();
    }
    if (argc != 2) {
      std::cout << "We need exactly one argument : use input_b.txt for Part b\n";
      return 1;
    }

    std::cout << run_game(argv[1]) << "\n";
    return 0;
}

int pass_tests() {
  int failing_tests = 0;

  failing_tests += test("../test_input_easy.txt", 32);
  failing_tests += test("../test_input_0.txt", 8317);
  failing_tests += test("../test_input_1.txt", 146373);
  failing_tests += test("../test_input_2.txt", 2764);
  failing_tests += test("../test_input_3.txt", 54718);
  failing_tests += test("../test_input_4.txt", 37305);
  failing_tests += test("../input.txt", 390093);
  failing_tests += test("../input_b.txt", 3150377341);

  return failing_tests;

}

int test(std::string filename, unsigned long long int expected) {
  auto result = run_game(filename.c_str());
  if (result == expected) {
    std::cout << filename << ": pass !\n";
    return 0;
  } else {
    std::cout << filename << ": ERROR -> expected " << expected << ", got " << result << "\n";
    return 1;
  }
}

unsigned long long int run_game(const char * filename) {
    size_t players;
    long long unsigned int last_marble;
    std::ifstream input_stream(filename);
    std::string _dummy;
    input_stream >> players >> _dummy >> _dummy >> _dummy >> _dummy >> _dummy >> last_marble >> _dummy;

    Game game(players, last_marble);
    game.play_game();
    return game.winner_score();

}
