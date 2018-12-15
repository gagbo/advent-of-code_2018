#include "catch2/catch.hpp"
#include "day15/entity.hpp"

TEST_CASE("ENTITY_CONSTRUCTION_FROM_CHAR", "[input]") {
    Entity wall('#');
  CHECK(wall.get_type() == Entity::Type::WALL);
    Entity goblin('G');
  CHECK(goblin.get_type() == Entity::Type::GOBLIN);
    Entity elf('E');
  CHECK(elf.get_type() == Entity::Type::ELF);
    Entity empty('.');
  CHECK(empty.get_type() == Entity::Type::VOID);
}