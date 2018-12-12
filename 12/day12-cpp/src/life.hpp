#ifndef LIFE_HPP__
#define LIFE_HPP__

#include <string>
#include <vector>

// A class containing the correspondance between patterns and state of the cell.
// At first it will only do one step at a time but should probably grow HashLife
// style
class Pattern {
 public:
    static const size_t PAT_COUNT = 32;
    static const size_t PAT_LEN = 5;

    Pattern() = default;
    void add_pattern_from_input(const std::string& input_line);
    bool pat[PAT_COUNT]{};
};

// A class containing the current state of the game and queryable for future
// states.
class LifeLine {
    // TODO : create iterator for LifeLine
 public:
    LifeLine(const std::string& input_line);
    short int& value(int game_index);
    short int value(int game_index) const;
    short int& next_value(int game_index);
    short int next_value(int game_index) const;
    void print() const;
    Pattern rules{};
    void advance();
    unsigned int turn{0};
    int sum_pots();

 private:
    // Vector containing R entirely. It should only be accessed through a method
    // to ensure correct indicies correspondance.
    // game -> data indices : ind => 2*ind if ind >= 0 else -2*ind-1
    // data -> game indices : ind => ind/2 if ind % 2 == 0 else -(ind+1)/2
    std::vector<short int>
        data;  // Can't use bool because the overload prevents returning
               // non-const bool& in value()
    std::vector<short int>
        next_data;  // Can't use bool because the overload prevents returning
                    // non-const bool& in value()
    int max_game_index{0};
    int min_game_index{0};

    void trim_data();
    // Compute all next values
    void tick();
    // Apply all next values
    void tock();
};

#endif  // LIFE_HPP
