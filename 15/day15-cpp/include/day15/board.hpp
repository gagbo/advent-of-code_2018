#ifndef DAY15_BOARD_HPP__
#define DAY15_BOARD_HPP__

#include "day15/entity.hpp"

class Board {
 public:
  Board(const char* filename);

  inline const pos_t get_dim() const { return dimensions; }

 protected:
  /**
   * @brief position of the max coord (X,Y) STILL IN the board
   *
   */
  pos_t dimensions;

  void add_line_as_coord_y(std::string input_line, unsigned int fixed_y);
};

#endif  // DAY15_BOARD_HPP__
