#include "game.hpp"
#include <cmath>

Game::Game(long long unsigned int wanted_score) {
    circle.assign({3, 7});
    p_cursor[0] = circle.begin();
    p_cursor[1] = circle.begin();
    ++p_cursor[1];
    current_turn = 0;
    target_circle_size = wanted_score + 10;

    std::cout << "The game will try to compute score of recipe " << wanted_score
              << "\n";
}

void Game::move_cursor(size_t player, int offset) {
    if (offset > 0) {
        for (int i = offset; i != 0; i--) {
            tick_cursor_tailwards(player);
        }
    } else if (offset < 0) {
        for (int i = offset; i != 0; i++) {
            tick_cursor_headwards(player);
        }
    }
}

void Game::tick_cursor_headwards(size_t player) {
    if (p_cursor[player] == circle.begin()) {
        p_cursor[player] = circle.end();
        --p_cursor[player];
    } else {
        --p_cursor[player];
    }
}
void Game::tick_cursor_tailwards(size_t player) {
    ++p_cursor[player];
    if (p_cursor[player] == circle.end()) {
        p_cursor[player] = circle.begin();
    }
}
void Game::print() const {
    std::cout << "\n"
                 "Game circle state :\n";

    for (auto recipe = circle.cbegin(); recipe != circle.cend(); recipe++) {
        if (recipe == p_cursor[0]) {
            std::cout << "_(" << *recipe << ")_ ";
        } else if (recipe == p_cursor[1]) {
            std::cout << "_[" << *recipe << "]_ ";
        } else {
            std::cout << *recipe << " ";
        }
    }

    std::cout << std::endl;
}

void Game::play_game() {
    while (circle.size() < target_circle_size) {
        do_one_turn();
    }
    std::cout << "Done in " << current_turn << " turns.\n";
}

void Game::do_one_turn() { current_turn++;

  auto old_p1_score = *p_cursor[0];
  auto old_p2_score = *p_cursor[1];

  auto new_recipe = static_cast<unsigned short int>(*p_cursor[0] + *p_cursor[1]);
  if (new_recipe < 10) {
    circle.push_back(new_recipe);
  } else {
    // Assuming max correct new_recipe value is 18
    circle.push_back(1);
    circle.push_back(new_recipe % 10);
  }

  move_cursor(0, 1 + old_p1_score);
  move_cursor(1, 1 + old_p2_score);
}

long long unsigned int Game::recipe_score(size_t recipe, unsigned short int digit_count) const {
    if (recipe + 10 > circle.size()) {
        std::cerr << "Asking for a too big recipe index\n";
        exit(1);
    }

    auto p_score = circle.begin();
    for (size_t i = 0; i < recipe; ++i) {
        p_score++;
    }

    long unsigned int result = 0;
    for (int i = digit_count - 1; i >= 0; --i) {
        result += static_cast<long unsigned int>(std::round(*p_score * std::pow(10, i)));
        p_score++;
    }
    return result;
}

long long unsigned int Game::last_n_digit_score(unsigned short int digit_count, size_t back_offset) const {
    if (digit_count + back_offset > circle.size()) {
      return 0;
    }

    long unsigned int result = 0;
    auto p_score = circle.rbegin();
    for (size_t i = 0 ; i < back_offset ; ++i) {
      p_score++;
    }
    for (size_t i = 0; i < digit_count; ++i) {
        result += static_cast<long unsigned int>(std::round(*p_score * std::pow(10, i)));
        p_score++;
    }
    return result;
}
