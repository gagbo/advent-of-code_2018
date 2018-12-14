#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "game.hpp"

static const unsigned long int PART_B_THRES = 80000000;
void part_a(unsigned long int recipes);
void part_b(unsigned long int wanted_score);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "We need exactly two arguments.\n";
        return 1;
    }

    if (strncmp(argv[1], "a", 1) == 0) {
        part_a(std::stoul(argv[2]));
        return 0;
    }
    if (strncmp(argv[1], "b", 1) == 0) {
        part_b(std::stoul(argv[2]));
        return 0;
    }

    return 1;
}

void part_a(unsigned long int recipes) {
    Game game(recipes);
    game.play_game();
    /* game.print(); */
    std::cout << "Score : " << game.recipe_score(recipes) << "\n";
}

void part_b(unsigned long int wanted_score) {
    std::cout << "Be careful that this won't exactly work as expected with "
                 "wanted scores starting with 0\n";
    auto digits_in_score =
        static_cast<unsigned short int>(std::ceil(std::log10(wanted_score)));
    Game game(PART_B_THRES);

    while (true) {
        game.do_one_turn();

        if (game.last_n_digit_score(digits_in_score, 1) == wanted_score) {
            std::cout << "Found the answer (in double addition) : "
                      << game.circle_size() - digits_in_score - 1 << "\n";
            break;
        }

        if (game.last_n_digit_score(digits_in_score, 0) == wanted_score) {
            std::cout << "Found the answer : "
                      << game.circle_size() - digits_in_score << "\n";
            break;
        }
    }
}
