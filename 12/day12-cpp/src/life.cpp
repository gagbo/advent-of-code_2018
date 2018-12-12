#include "life.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <numeric>

LifeLine::LifeLine(const std::string& input_line) {
    std::istringstream input(input_line);
    std::string state;
    input >> state >> state >> state;  // Ignore first 2 words
    int game_index = 0;
    for (char c : state) {
        value(game_index) = (c == '#') ? 1 : 0;
        game_index++;
    }
}

void LifeLine::print() const {
    std::cout << "Turn " << turn << "\n";
    std::cout << "Game spans from " << min_game_index << " to "
              << max_game_index << "\n";
    //for (int i = min_game_index; i <= max_game_index; ++i) {
    //    std::cout << ((value(i) == 1) ? "#" : ".");
    //}
    std::cout << "\n";
}
short int& LifeLine::value(int game_index) {
    if (game_index >= 0 && static_cast<size_t>(2 * game_index) >= data.size()) {
        data.resize(static_cast<size_t>(2 * game_index + 1), 0);
        max_game_index = game_index;
    }
    if (game_index < 0 &&
        static_cast<size_t>(-2 * game_index - 1) >= data.size()) {
        data.resize(static_cast<size_t>(-2 * game_index), 0);
        min_game_index = game_index;
    }
    return game_index >= 0 ? data[static_cast<size_t>(2 * game_index)]
                           : data[static_cast<size_t>(-2 * game_index - 1)];
}

short int LifeLine::value(int game_index) const {
    if (game_index >= 0 && static_cast<size_t>(2 * game_index) >= data.size())
        return 0;
    if (game_index < 0 &&
        static_cast<size_t>(-2 * game_index - 1) >= data.size())
        return 0;
    return game_index >= 0 ? data[static_cast<size_t>(2 * game_index)]
                           : data[static_cast<size_t>(-2 * game_index - 1)];
}

short int& LifeLine::next_value(int game_index) {
    if (game_index >= 0 &&
        static_cast<size_t>(2 * game_index) >= next_data.size()) {
        next_data.resize(static_cast<size_t>(2 * game_index + 1), 0);
        max_game_index = game_index;
    }
    if (game_index < 0 &&
        static_cast<size_t>(-2 * game_index - 1) >= next_data.size()) {
        next_data.resize(static_cast<size_t>(-2 * game_index), 0);
        min_game_index = game_index;
    }
    return game_index >= 0
               ? next_data[static_cast<size_t>(2 * game_index)]
               : next_data[static_cast<size_t>(-2 * game_index - 1)];
}

short int LifeLine::next_value(int game_index) const {
    if (game_index >= 0 &&
        static_cast<size_t>(2 * game_index) >= next_data.size())
        return 0;
    if (game_index < 0 &&
        static_cast<size_t>(-2 * game_index - 1) >= next_data.size())
        return 0;
    return game_index >= 0
               ? next_data[static_cast<size_t>(2 * game_index)]
               : next_data[static_cast<size_t>(-2 * game_index - 1)];
}

void Pattern::add_pattern_from_input(const std::string& input_line) {
    size_t pat_index = 0;
    for (size_t i = 0; i < PAT_LEN; ++i) {
        if (input_line[i] == '#') {
            pat_index += static_cast<size_t>(std::pow(2, PAT_LEN - 1 - i));
        }
    }
    pat[pat_index] = input_line[9] == '#';
    std::cerr << "Pattern added : " << pat_index << " : " << pat[pat_index]
              << "\n";
}

void LifeLine::advance() {
    tick();
    tock();
}

void LifeLine::tick() {
    int left_expansion = 2;
    int right_expansion = 2;
    int fixed_min = min_game_index - left_expansion;
    int fixed_max = max_game_index + right_expansion;
    for (int i = fixed_min; i <= fixed_max; ++i) {
        size_t pat_index = 0;
        for (size_t offset = 0; offset < Pattern::PAT_LEN; ++offset) {
            if (value(i + 2 - offset)) {
                pat_index += static_cast<size_t>(std::pow(2, offset));
            }
        }

        next_value(i) = rules.pat[pat_index] ? 1 : 0;
    }
}

void LifeLine::tock() {
    int fixed_min = min_game_index;
    int fixed_max = max_game_index;
    for (int i = fixed_min; i <= fixed_max; ++i) {
        value(i) = next_value(i);
    }
    ++turn;

    trim_data();
}

void LifeLine::trim_data() {
    auto last_true = std::find_if(data.rbegin(), data.rend(),
                                  [](int val) -> bool { return val != 0; });
    if (last_true != data.rend()) {
        data.erase(last_true.base() + 1, data.end());
        data.shrink_to_fit();
    }

    if (data.size() % 2 == 0) {
      min_game_index = - data.size() / 2;
      max_game_index = - min_game_index;
    } else {
      max_game_index = (data.size() + 1)/2;
      min_game_index = - max_game_index;
    }
}

int LifeLine::sum_pots() {
    int fixed_min = min_game_index;
    int fixed_max = max_game_index;
    int result = 0;
    for (int i = fixed_min; i <= fixed_max; ++i) {
        if(value(i)) {
          result += i;
        }
    }
    return result;
}
