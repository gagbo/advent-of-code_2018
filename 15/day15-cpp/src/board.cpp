#include "day15/board.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

Board::Board(const char *filename) {
  std::ifstream input_stream(filename);
  if (!input_stream) {
    std::cerr << "Error opening " << filename << "\n";
    exit(1);
  }
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
        free_space[current_pos] = false;
        break;
      case 'G':
        living_entities.insert(std::make_shared<Entity>(c, current_pos));
        free_space[current_pos] = false;
        break;
      case 'E':
        living_entities.insert(std::make_shared<Entity>(c, current_pos));
        free_space[current_pos] = false;
        break;
      case '.':
        free_space[current_pos] = true;
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

std::optional<unsigned int> Board::distance_between(pos_t point_a,
                                                    pos_t point_b) const {
  if (!free_space.at(point_b)) return std::nullopt;
  if (point_a == point_b) return 0;

  auto size_i = dimensions.first + 1;
  auto size_j = dimensions.second + 1;
  std::vector<std::optional<unsigned int>> dist_value(size_i * size_j,
                                                      std::nullopt);

  dist_value[point_b.first * size_j + point_b.second] = 0;
  bool value_changed = true;
  while (value_changed) {
    value_changed = false;
    for (size_t i = 0; i < size_i; ++i) {
      for (size_t j = 0; j < size_j; ++j) {
        auto neighbour_n = (!is_free(i, j - 1))
                               ? std::nullopt
                               : dist_value[i * size_j + (j - 1)];
        auto neighbour_w = (!is_free(i - 1, j))
                               ? std::nullopt
                               : dist_value[(i - 1) * size_j + j];
        auto neighbour_e = (!is_free(i + 1, j))
                               ? std::nullopt
                               : dist_value[(i + 1) * size_j + j];
        auto neighbour_s = (!is_free(i, j + 1))
                               ? std::nullopt
                               : dist_value[i * size_j + (j + 1)];

        if (!neighbour_w && !neighbour_e && !neighbour_s && !neighbour_n) {
          dist_value[i * size_j + j] = std::nullopt;
          continue;
        }

        auto old_val = dist_value[i * size_j + j];

        dist_value[i * size_j + j] = std::min(
            neighbour_n.value_or(std::numeric_limits<unsigned int>::max() - 1) +
                1,
            dist_value[i * size_j + j].value_or(
                std::numeric_limits<unsigned int>::max()));
        dist_value[i * size_j + j] = std::min(
            neighbour_w.value_or(std::numeric_limits<unsigned int>::max() - 1) +
                1,
            dist_value[i * size_j + j].value_or(
                std::numeric_limits<unsigned int>::max()));
        dist_value[i * size_j + j] = std::min(
            neighbour_e.value_or(std::numeric_limits<unsigned int>::max() - 1) +
                1,
            dist_value[i * size_j + j].value_or(
                std::numeric_limits<unsigned int>::max()));
        dist_value[i * size_j + j] = std::min(
            neighbour_s.value_or(std::numeric_limits<unsigned int>::max() - 1) +
                1,
            dist_value[i * size_j + j].value_or(
                std::numeric_limits<unsigned int>::max()));

        dist_value[i * size_j + j] = (dist_value[i * size_j + j] ==
                                      std::numeric_limits<unsigned int>::max())
                                         ? std::nullopt
                                         : dist_value[i * size_j + j];

        value_changed = (old_val != dist_value[i * size_j + j]);
      }
    }
  }

  return dist_value[point_a.first * size_j + point_a.second];
}
