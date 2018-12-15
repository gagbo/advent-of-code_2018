#include "catch2/catch.hpp"
#include "day15/lib.hpp"

TEST_CASE("BASIC_TEST", "[basic]") {
  Dummy instance;
  CHECK(instance.get_name() == "Dummy");
}
