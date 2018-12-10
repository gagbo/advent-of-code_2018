#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <string>

#include "sky.hpp"

void watch_sky(const char* filename);

int main(int argc, char *argv[])
{
    if (argc != 3) {
      std::cout << "We need exactly two arguments.\n";
      return 1;
    }

    if (strncmp(argv[1], "a", 1) == 0) {
      watch_sky(argv[2]);
      return 0;
    }

  return 1;
}

void watch_sky(const char* filename) {
    std::ifstream input_stream(filename);
    std::string line;
    std::vector<Star> star_list;
    while ( std::getline(input_stream, line)) {
      star_list.emplace_back(line);
    }

    int counter = 0;
    Sky sky(std::move(star_list));
    while ( sky.area() > sky.fut_area()) {
      sky.advance();
      ++counter;
    }
    std::cout << "Stopped at " << counter << " : \n";
    sky.print();
}
