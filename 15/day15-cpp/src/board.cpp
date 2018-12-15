#include "day15/board.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Board::Board(const char *filename) {
  std::ifstream input_stream(filename);
  std::string line;
  unsigned int y = 0;
  while (std::getline(input_stream, line)) {
    add_line_as_coord_y(line, y);
    ++y;
  }
}

void Board::add_line_as_coord_y(std::string input_line, unsigned int fixed_y) {
  unsigned int x = 0;
  for (auto c : input_line) {
    pos_t current_pos = std::make_pair(x, fixed_y);
    switch (c) {
      case '#':
        std::cout << "Wall ";
        break;
      case 'G':
        std::cout << "Gobl ";
        break;
      case 'E':
        std::cout << "Elf  ";
        break;
      case '.':
        std::cout << "Void ";
        break;
      default:
        break;
    }
    ++x;
  }
  dimensions.first = std::max(dimensions.first, x - 1);
  dimensions.second = std::max(dimensions.second, fixed_y);
  std::cout << "\n";
}
