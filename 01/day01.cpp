#include <cstring>
#include <iostream>
#include <set>
#include <string>
#include <vector>

long long int part_a();
long long int part_b();

int main(int argc, char *argv[]) {
  long long int frequency = 0;
  if (argc != 2) {
    std::cerr << "Wrong number of arguments !"
                 "Usage : "
              << argv[0] << " [a,b]\n";
    exit(1);
  }

  if (std::strncmp(argv[1], "a", 1) == 0) {
    frequency = part_a();
  } else if (std::strncmp(argv[1], "b", 1) == 0) {
    frequency = part_b();
  }

  std::cout << frequency << "\n";
  return 0;
}

long long int part_a() {
  std::cout << "Calling part a\n";
  long long int starting_frequency = 0;
  std::string line;

  while (std::getline(std::cin, line)) {
    long long int offset = std::stoll(line);
    starting_frequency += offset;
  }
  return starting_frequency;
}

long long int part_b() {
  std::cout << "Calling part b\n";
  long starting_frequency = 0;
  std::set<long long int> visited_frequencies;
  std::string line;

  std::vector<long long int> input_to_loop;

  visited_frequencies.insert(starting_frequency);
  while (std::getline(std::cin, line)) {
    long long int offset = std::stoll(line);
    starting_frequency += offset;
    auto result = visited_frequencies.insert(starting_frequency);
    if (!result.second) {
      std::cout << "Duplicate found !";
      return starting_frequency;
    }
    input_to_loop.push_back(offset);
  }

  std::cout << "Starting to repeat input\n";
  int loop_index = 1;
  while (true) {
    std::cout << "Re-loop number : " << loop_index << "\n";
    for (long long int offset : input_to_loop) {
      starting_frequency += offset;
      auto result = visited_frequencies.insert(starting_frequency);
      if (!result.second) {
        std::cout << "Duplicate found !\n";
        return starting_frequency;
      }
    }
    loop_index++;
  }

  return starting_frequency;
}
