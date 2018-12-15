#include "catch2/catch.hpp"
#include "day15/board.hpp"

TEST_CASE("BOARD_READ_FROM_FILE", "[input]") {
  Board board_0("../input/test_input_0.txt");
  CHECK(board_0.get_dim().first == 6);
  CHECK(board_0.get_dim().second == 6);
  Board board_1("../input/test_input_1.txt");
  CHECK(board_1.get_dim().first == 6);
  CHECK(board_1.get_dim().second == 6);
  Board board_2("../input/test_input_2.txt");
  CHECK(board_2.get_dim().first == 6);
  CHECK(board_2.get_dim().second == 6);
  Board board_3("../input/test_input_3.txt");
  CHECK(board_3.get_dim().first == 6);
  CHECK(board_3.get_dim().second == 6);
  Board board_4("../input/test_input_4.txt");
  CHECK(board_4.get_dim().first == 6);
  CHECK(board_4.get_dim().second == 6);
  Board board_5("../input/test_input_5.txt");
  CHECK(board_5.get_dim().first == 8);
  CHECK(board_5.get_dim().second == 8);
  Board board_input("../input/input.txt");
  CHECK(board_input.get_dim().first == 31);
  CHECK(board_input.get_dim().second == 31);
}
