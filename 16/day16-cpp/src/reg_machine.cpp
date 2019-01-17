#include "reg_machine.hpp"

#include <iostream>
uint16_t &Opcode::operator[](size_t i) {
  switch (i) {
  case 0:
    return opcode;
    break;
  case 1:
    return field_0;
    break;
  case 2:
    return field_1;
    break;
  case 3:
    return field_r;
    break;
  default:
    std::cout << "called for a bad field\n";
    std::exit(1);
  }
}
