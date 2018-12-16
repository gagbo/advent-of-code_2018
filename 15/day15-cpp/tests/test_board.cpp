#include "catch2/catch.hpp"
#include "day15/board.hpp"

TEST_CASE("BOARD_READ_DIMENSIONS", "[input]") {
  SECTION("BOARD_0") {
    Board board_0("../input/test_input_0.txt");
    CHECK(board_0.get_dim().first == 6);
    CHECK(board_0.get_dim().second == 6);
  }
  SECTION("BOARD_1") {
    Board board_1("../input/test_input_1.txt");
    CHECK(board_1.get_dim().first == 6);
    CHECK(board_1.get_dim().second == 6);
  }
  SECTION("BOARD_2") {
    Board board_2("../input/test_input_2.txt");
    CHECK(board_2.get_dim().first == 6);
    CHECK(board_2.get_dim().second == 6);
  }
  SECTION("BOARD_3") {
    Board board_3("../input/test_input_3.txt");
    CHECK(board_3.get_dim().first == 6);
    CHECK(board_3.get_dim().second == 6);
  }
  SECTION("BOARD_4") {
    Board board_4("../input/test_input_4.txt");
    CHECK(board_4.get_dim().first == 6);
    CHECK(board_4.get_dim().second == 6);
  }
  SECTION("BOARD_5") {
    Board board_5("../input/test_input_5.txt");
    CHECK(board_5.get_dim().first == 8);
    CHECK(board_5.get_dim().second == 8);
  }
  SECTION("BOARD_input") {
    Board board_input("../input/input.txt");
    CHECK(board_input.get_dim().first == 31);
    CHECK(board_input.get_dim().second == 31);
  }
}

TEST_CASE("BOARD_INITIAL_FREE_SPACE", "[input]") {
  SECTION("BOARD_0") {
    Board board_0("../input/test_input_0.txt");
    CHECK(board_0.is_free(0, 0) == false);
    CHECK(board_0.is_free(1, 0) == false);
    CHECK(board_0.is_free(2, 0) == false);
    CHECK(board_0.is_free(3, 0) == false);
    CHECK(board_0.is_free(4, 0) == false);
    CHECK(board_0.is_free(5, 0) == false);
    CHECK(board_0.is_free(6, 0) == false);
    CHECK(board_0.is_free(0, 1) == false);
    CHECK(board_0.is_free(1, 1) == true);
    CHECK(board_0.is_free(2, 1) == false);
    CHECK(board_0.is_free(3, 1) == true);
    CHECK(board_0.is_free(4, 1) == true);
    CHECK(board_0.is_free(5, 1) == true);
    CHECK(board_0.is_free(6, 1) == false);
    CHECK(board_0.is_free(0, 2) == false);
    CHECK(board_0.is_free(1, 2) == true);
    CHECK(board_0.is_free(2, 2) == true);
    CHECK(board_0.is_free(3, 2) == true);
    CHECK(board_0.is_free(4, 2) == false);
    CHECK(board_0.is_free(5, 2) == false);
    CHECK(board_0.is_free(6, 2) == false);
    CHECK(board_0.is_free(0, 3) == false);
    CHECK(board_0.is_free(1, 3) == true);
    CHECK(board_0.is_free(2, 3) == false);
    CHECK(board_0.is_free(3, 3) == true);
    CHECK(board_0.is_free(4, 3) == false);
    CHECK(board_0.is_free(5, 3) == false);
    CHECK(board_0.is_free(6, 3) == false);
    CHECK(board_0.is_free(0, 4) == false);
    CHECK(board_0.is_free(1, 4) == true);
    CHECK(board_0.is_free(2, 4) == true);
    CHECK(board_0.is_free(3, 4) == false);
    CHECK(board_0.is_free(4, 4) == false);
    CHECK(board_0.is_free(5, 4) == false);
    CHECK(board_0.is_free(6, 4) == false);
    CHECK(board_0.is_free(0, 5) == false);
    CHECK(board_0.is_free(1, 5) == true);
    CHECK(board_0.is_free(2, 5) == true);
    CHECK(board_0.is_free(3, 5) == true);
    CHECK(board_0.is_free(4, 5) == true);
    CHECK(board_0.is_free(5, 5) == true);
    CHECK(board_0.is_free(6, 5) == false);
    CHECK(board_0.is_free(0, 6) == false);
    CHECK(board_0.is_free(1, 6) == false);
    CHECK(board_0.is_free(2, 6) == false);
    CHECK(board_0.is_free(3, 6) == false);
    CHECK(board_0.is_free(4, 6) == false);
    CHECK(board_0.is_free(5, 6) == false);
    CHECK(board_0.is_free(6, 6) == false);
  }
}

TEST_CASE("BOARD_DISTANCE", "[input]") {
  SECTION("BOARD_0") {
    Board board_0("../input/test_input_0.txt");
    CHECK(board_0.distance_between(std::make_pair(1, 1), std::make_pair(1, 1))
              .value() == 0);
    CHECK(board_0.distance_between(std::make_pair(1, 1),
                                   std::make_pair(0, 0)) == std::nullopt);
    CHECK(board_0.distance_between(std::make_pair(2, 1), std::make_pair(4, 1))
              .value() == 2);
    CHECK(board_0.distance_between(std::make_pair(5, 4), std::make_pair(3, 3))
              .value() == 11);
  }
}