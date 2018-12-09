#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <string>

int pass_tests();
int test(std::string filename, unsigned long long expected);
unsigned long long int run_game(const char* filename);

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

Game::Game(size_t players, int last_marble) {
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

int pass_tests() {
    /* std::cout << "Rappel :\n" */
    /*              "9 ; 25 -> 32\n" */
    /*              "10 ; 1618 -> 8317\n" */
    /*              "13 ; 7999 -> 146373\n" */
    /*              "17 ; 1104 -> 2764\n" */
    /*              "21 ; 6111 -> 54718\n" */
    /*              "30 ; 5807 -> 37305\n" */
    /*              "471 ; 72026 -> 390093\n" */
    /*              "471 ; 7202600 -> 3150377341\n"; */
    /* std::cout << "Player count : "; */
    /* std::cin >> players; */
    /* std::cin.ignore(); */
    /* std::cout << "Marble count : "; */
    /* std::cin >> last_marble; */
    /* std::cin.ignore(); */
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
    int last_marble;
    std::ifstream input_stream(filename);
    std::string _dummy;
    input_stream >> players >> _dummy >> _dummy >> _dummy >> _dummy >> _dummy >> last_marble >> _dummy;

    Game game(players, last_marble);
    game.play_game();
    return game.winner_score();

}
