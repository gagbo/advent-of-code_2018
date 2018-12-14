#ifndef GAME_HPP__
#define GAME_HPP__
#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

class Game {
 public:
    Game(long long unsigned int wanted_score);
    void print() const;

    void play_game();
    void do_one_turn();
    inline size_t circle_size() const {return circle.size(); }
    long long unsigned int recipe_score(size_t recipe, unsigned short int digit_count = 10) const;

    long long unsigned int last_n_digit_score(unsigned short int digit_count, size_t back_offset) const;
 private:
    void move_cursor(size_t player, int offset);
    void tick_cursor_headwards(size_t player);
    void tick_cursor_tailwards(size_t player);
    std::list<short unsigned int> circle;
    std::list<short unsigned int>::iterator p_cursor[2];
    long long unsigned int current_turn;
    long long unsigned int target_circle_size;
};
#endif  // GAME_HPP__
