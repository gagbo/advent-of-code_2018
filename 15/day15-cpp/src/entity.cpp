#include "day15/entity.hpp"

Entity::Entity(char symbol) {
  switch (symbol) {
    case '#':
      type = Type::WALL;
      break;
    case 'E':
      type = Type::ELF;
      break;
    case 'G':
      type = Type::GOBLIN;
      break;
    case '.':
      type = Type::VOID;
      break;
  }
}
