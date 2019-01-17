#ifndef REG_MACHINE_HPP
#define REG_MACHINE_HPP

#include <array>
#include <functional>
#include <type_traits>

static const uint8_t ISA_SIZE = 16;

using Registers = std::array<uint32_t, 4>;
struct Opcode {
  uint16_t opcode;
  uint16_t field_0;
  uint16_t field_1;
  uint16_t field_r;

  uint16_t &operator[](size_t i);
};

using Instruction = std::function<Registers(Registers, Opcode)>;
struct NamedInstruction {
  char name[5];
  Instruction instr;
};

static const NamedInstruction INSTRUCTION_SET[ISA_SIZE] = {
    {"addr",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = before[op.field_0] + before[op.field_1];
       return result;
     }},
    {"addi",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = before[op.field_0] + op.field_1;
       return result;
     }},
    {"mulr",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = before[op.field_0] * before[op.field_1];
       return result;
     }},
    {"muli",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = before[op.field_0] * op.field_1;
       return result;
     }},
    {"banr",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = before[op.field_0] & before[op.field_1];
       return result;
     }},
    {"bani",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = before[op.field_0] & op.field_1;
       return result;
     }},
    {"borr",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = before[op.field_0] | before[op.field_1];
       return result;
     }},
    {"bori",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = before[op.field_0] | op.field_1;
       return result;
     }},
    {"setr",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = before[op.field_0];
       return result;
     }},
    {"seti",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = op.field_0;
       return result;
     }},
    {"gtir",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = op.field_0 > before[op.field_1] ? 1 : 0;
       return result;
     }},
    {"gtri",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = before[op.field_0]  > op.field_1 ? 1 : 0;
       return result;
     }},
    {"gtrr",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = before[op.field_0] > before[op.field_1] ? 1 : 0;
       return result;
     }},
    {"eqir",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = op.field_0 == before[op.field_1] ? 1 : 0;
       return result;
     }},
    {"eqri",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = before[op.field_0] == op.field_1 ? 1 : 0;
       return result;
     }},
    {"eqrr",
     [](Registers before, Opcode op) {
       auto result = before;
       result[op.field_r] = before[op.field_0] == before[op.field_1] ? 1 : 0;
       return result;
     }}
};

#endif // REG_MACHINE_HPP
