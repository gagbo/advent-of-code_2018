#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void part_a(const char* filename);
void part_b(const char* filename);

int main(int argc, char* argv[]) {
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

void part_a(const char* filename) {
    std::ifstream input_stream(filename);
    std::string line;
    std::cout << "unimplemented\n";
    while ( std::getline(input_stream, line)) {
      // Read the tracks line by line
    }
}

void part_b(const char* filename) {
    std::ifstream input_stream(filename);
    std::string line;
    std::cout << "unimplemented\n";
    while ( std::getline(input_stream, line)) {
      // Read the tracks line by line
    }
}
