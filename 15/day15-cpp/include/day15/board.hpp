#ifndef DAY15_BOARD_HPP__
#define DAY15_BOARD_HPP__

#include <map>
#include <memory>
#include <optional>
#include <set>
#include "day15/entity.hpp"

/**
 * @brief Comparison Struct for evaluation order on the board
 *
 */
struct EntityPtrEvalCmp {
  bool operator()(const std::shared_ptr<Entity> &lhs,
                  const std::shared_ptr<Entity> &rhs) const {
    return lhs.get()->before_in_reading_order(*rhs.get());
  }
};

class Board {
 public:
  Board(const char *filename);

  inline const pos_t get_dim() const { return dimensions; }

  /**
   * @brief Return true if the position on the board is free
   *
   * @param position queried position
   * @return true if an entity can move there
   * @return false if there is already something there
   */
  inline bool is_free(pos_t position) const {
    try {
      return free_space.at(position);
    } catch (const std::out_of_range &e) {
      return false;
    }
  }
  /**
   * @brief Return true if the position on the board is free
   *
   * @param x queried position abscissa
   * @param y queried position ordinate
   * @return true if an entity can move there
   * @return false if there is already something there
   */
  inline bool is_free(unsigned int x, unsigned int y) const {
    return is_free(std::make_pair(x, y));
  }

  /**
   * @brief compute the distance along a free path between point_a and point_b
   *
   * @param point_a origin of the distance, free_space[point_a] can be false
   * @param point_b destination of the distance, free_space[point_b] must be
   * true
   * @return std::optional<unsigned int>  is none if no free path exists,
   * contains the distance otherwise
   */
  std::optional<unsigned int> distance_between(pos_t point_a,
                                               pos_t point_b) const;

  /**
   * @brief Do one turn of the big fight
   *
   */
  void do_one_turn();
  /**
   * @brief Run all turns until one faction wins
   *
   */
  void play_game();
  /**
   * @brief Print the current state of the game
   *
   */
  void print() const;

 protected:
  /**
   * @brief position of the max coord (X,Y) STILL IN the board
   *
   */
  pos_t dimensions;

  /**
   * @brief Add the line in argument into the current board, with a fixed y
   * coordinate
   *
   * @param input_line the line to read as a y-line
   * @param fixed_y  the value of the ordinate for this y-line
   */
  void add_line_as_coord_y(std::string input_line, unsigned int fixed_y);

  /**
   * @brief Entites which take action during a turn
   *
   */
  std::set<std::shared_ptr<Entity>, EntityPtrEvalCmp> living_entities;

  std::map<pos_t, bool> free_space;

  unsigned int turn;
};

#endif  // DAY15_BOARD_HPP__
