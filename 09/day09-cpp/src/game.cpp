#include "game.h"

Game::Game(size_t players, long long unsigned int last_marble) {
    scores = std::vector<long long unsigned int>(players);
    circle = std::list<long long unsigned int>(1);
    circle_cursor = circle.begin();
    current_turn = 0;
    player_cursor = 0;
    last_turn = last_marble;
    manual_mod_count = 0;

    std::cout << "Starting game with " << scores.size() << " players and "
              << last_marble << " marbles\n";
}

void Game::move_cursor(int offset) {
    if (offset > 0) {
        for (int i = offset; i != 0; i--) {
            tick_cursor_tailwards();
        }
    } else if (offset < 0) {
        for (int i = offset; i != 0; i++) {
            tick_cursor_headwards();
        }
    }
}

void Game::tick_cursor_headwards() {
    if (circle_cursor == circle.begin()) {
        circle_cursor = circle.end();
        --circle_cursor;
    } else {
        --circle_cursor;
    }
}
void Game::tick_cursor_tailwards() {
    ++circle_cursor;
    if (circle_cursor == circle.end()) {
        circle_cursor = circle.begin();
    }
}
void Game::print() const {
    std::cout << "Game scores :\n";
    for (auto score : scores) {
        std::cout << score << " ";
    }
    std::cout << "\n"
                 "Game circle state :\n";
    if (current_turn == 0) {
        std::cout << "[-]: ";
    } else {
        std::cout << "[" << player_cursor << "]: ";
    }
    for (auto marble = circle.cbegin(); marble != circle.cend(); marble++) {
        if (marble == circle_cursor) {
            std::cout << "_(" << *marble << ")_ ";
        } else {
            std::cout << *marble << " ";
        }
    }
    std::cout << std::endl;
}

void Game::play_game() {
    while (current_turn < last_turn) {
        do_one_turn();
    }
}

void Game::do_one_turn() {
    current_turn++;
    manual_mod_count++;
    // Do not advance player_cursor on first turn
    if (current_turn == 1) {
        player_cursor = 0;
    } else {
        player_cursor += 1;
        player_cursor = (player_cursor >= scores.size()) ? 0 : player_cursor;
    }

    if (manual_mod_count == 23) {
        move_cursor(-7);
        scores[player_cursor] += current_turn;
        scores[player_cursor] += *circle_cursor;
        circle_cursor = circle.erase(circle_cursor);
        manual_mod_count = 0;
    } else {
        move_cursor(2);
        circle_cursor = circle.insert(circle_cursor, current_turn);
    }
}

long long unsigned int Game::winner_score() const {
    return *std::max_element(scores.cbegin(), scores.cend());
}
