#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "reg_machine.hpp"

void part_a(const char *filename);
void part_b(const char *filename);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << "We need exactly two arguments.\n";
    return 1;
  }

  if (strncmp(argv[1], "a", 1) == 0) {
    part_a(argv[2]);
    return 0;
  }
  if (strncmp(argv[1], "b", 1) == 0) {
    part_b(argv[2]);
    return 0;
  }

  return 1;
}

void part_a(const char *filename) {
  std::ifstream input_stream(filename);
  std::string line;
  int16_t matches_3_ops_or_more{0};
  while (std::getline(input_stream, line)) {
    auto pos = line.find("Before: ");
    if (pos == std::string::npos) {
      break;
    }
    Registers regs_before;
    Opcode operation;
    Registers regs_after;
    auto regs = line.substr(line.find("["));
    char sep;
    std::istringstream(regs) >> sep >> regs_before[0] >> sep >>
        regs_before[1] >> sep >> regs_before[2] >> sep >> regs_before[3];
    std::getline(input_stream, line);
    auto line_stream = std::istringstream(line);
    line_stream >> operation[0] >> operation[1] >> operation[2] >> operation[3];
    std::getline(input_stream, line);
    regs = line.substr(line.find("["));
    std::istringstream(regs) >> sep >> regs_after[0] >> sep >> regs_after[1] >>
        sep >> regs_after[2] >> sep >> regs_after[3];

    int matching_ops = 0;
    for (size_t i = 0; i < ISA_SIZE; ++i) {
      if (INSTRUCTION_SET[i].instr(regs_before, operation) == regs_after) {
        std::cout << INSTRUCTION_SET[i].name << " ";
        matching_ops++;
      }
    }
    std::cout << "matched !\n";

    std::cout << "Registers before : \n"
              << regs_before[0] << " " << regs_before[1] << " "
              << regs_before[2] << " " << regs_before[3] << "\n";
    std::cout << "Operation : \n"
              << operation[0] << " " << operation[1] << " " << operation[2]
              << " " << operation[3] << "\n";
    std::cout << "Registers after : \n"
              << regs_after[0] << " " << regs_after[1] << " " << regs_after[2]
              << " " << regs_after[3] << "\n";
    std::cout << "Matched " << matching_ops << " operations\n\n";
    if (matching_ops >= 3)
      ++matches_3_ops_or_more;
    input_stream.ignore();
  }
  std::cout << matches_3_ops_or_more << "\n";
}

void part_b(const char *filename) {
  std::ifstream input_stream(filename);
  std::string line;
  std::array<std::array<bool, ISA_SIZE>, ISA_SIZE> valid_instr_for_opcode;
  // Init the array at all true
  for (auto &opcode_truth_table : valid_instr_for_opcode) {
    for (auto &is_instr_possible : opcode_truth_table) {
      is_instr_possible = true;
    }
  }

  // First parsing of diagnostics to rule out as many opcode <-> instruction
  // pairs as possible
  while (std::getline(input_stream, line)) {
    auto pos = line.find("Before: ");
    if (pos == std::string::npos) {
      break;
    }
    Registers regs_before;
    Opcode operation;
    Registers regs_after;
    // This Registers + op reading would be better in the class, but too much
    // hassle to parse strings correctly. Maybe there's a regex lib I could
    // use ?
    auto regs = line.substr(line.find("["));
    char sep;
    std::istringstream(regs) >> sep >> regs_before[0] >> sep >>
        regs_before[1] >> sep >> regs_before[2] >> sep >> regs_before[3];
    std::getline(input_stream, line);
    auto line_stream = std::istringstream(line);
    line_stream >> operation[0] >> operation[1] >> operation[2] >> operation[3];
    std::getline(input_stream, line);
    regs = line.substr(line.find("["));
    std::istringstream(regs) >> sep >> regs_after[0] >> sep >> regs_after[1] >>
        sep >> regs_after[2] >> sep >> regs_after[3];

    for (size_t i = 0; i < ISA_SIZE; ++i) {
      if (INSTRUCTION_SET[i].instr(regs_before, operation) != regs_after) {
        valid_instr_for_opcode[operation.opcode][i] = false;
      }
    }

    input_stream.ignore();
  }

#ifndef NDEBUG
  int j = 0;
  for (auto opcode_truth_table : valid_instr_for_opcode) {
    std::cerr << j << " : ";
    for (auto val : opcode_truth_table) {
      std::cerr << val << " ";
    }
    std::cerr << "\n";
    ++j;
  }
#endif // NDEBUG

  // Should read as "As long as at least one opcode has more than 1 possible
  // instruction"
  std::cerr << "Deducing instruction set\n";
  while (std::count_if(valid_instr_for_opcode.begin(),
                       valid_instr_for_opcode.end(),
                       [](auto opcode_truth_table) {
                         return std::count_if(opcode_truth_table.begin(),
                                              opcode_truth_table.end(),
                                              [](auto val) { return val; }) > 1;
                       })) {
    for (size_t opcode = 0; opcode < ISA_SIZE; ++opcode) {
      auto opcode_truth_table = valid_instr_for_opcode[opcode];
      auto true_values =
          std::count_if(opcode_truth_table.begin(), opcode_truth_table.end(),
                        [](auto val) { return val; });

      if (true_values == 1) {
        auto it_true = std::find(opcode_truth_table.begin(),
                                 opcode_truth_table.end(), true);
        auto index_true = static_cast<size_t>(std::abs(std::distance(opcode_truth_table.begin(), it_true)));
#ifndef NDEBUG
        auto named_inst = INSTRUCTION_SET[index_true];
        auto name = named_inst.name;
        std::cerr << "Found a match for " << opcode << " : " << index_true
                  << "->" << name << "\n";
#endif // NDEBUG

        for (size_t other_opcode = 0; other_opcode < ISA_SIZE; ++other_opcode) {
          if (opcode == other_opcode)
            continue;
          valid_instr_for_opcode[other_opcode][index_true] = false;
        }
      }
    }
  }

  input_stream.ignore();
  input_stream.ignore();
  Registers current_regs{0, 0, 0, 0};
  while (std::getline(input_stream, line)) {
    Opcode operation;
    auto line_stream = std::istringstream(line);
    line_stream >> operation[0] >> operation[1] >> operation[2] >> operation[3];
    auto opcode_lookup = valid_instr_for_opcode[operation.opcode];
    auto it_true = std::find(opcode_lookup.begin(), opcode_lookup.end(), true);
    auto index_true = std::distance(opcode_lookup.begin(), it_true);
    auto named_inst = INSTRUCTION_SET[index_true];
#ifndef NDEBUG
    auto name = named_inst.name;
    std::cerr << "Applying " << name << "\n";
#endif // NDEBUG
    current_regs = named_inst.instr(current_regs, operation);
  }
  std::cout << "Registers : " << current_regs[0] << " " << current_regs[1]
            << " " << current_regs[2] << " " << current_regs[3] << "\n";
}
