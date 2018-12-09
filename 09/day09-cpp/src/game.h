#ifndef GAME_H__
#define GAME_H__
#include <algorithm>
#include <list>
#include <vector>
#include <iostream>

class Game {
 public:
    Game(size_t players, int last_marble);
    void print() const;

    void play_game();
    long long unsigned int winner_score() const;

 private:
    void do_one_turn();
    void move_cursor(int offset);
    void tick_cursor_headwards();
    void tick_cursor_tailwards();
    std::vector<long long unsigned int> scores;
    std::list<long long unsigned int> circle;
    std::list<long long unsigned int>::iterator circle_cursor;
    long long unsigned int current_turn;
    int player_cursor;
    long long unsigned int last_turn;
    int manual_mod_count;
};
#endif  // GAME_H__
