#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

#include "tracks.hpp"

void part_a(const char *filename);
void part_b(const char *filename);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "We need exactly two arguments.\n";
        return 1;
    }

    if (strncmp(argv[1], "a", 1) == 0)
    {
        part_a(argv[2]);
        return 0;
    }
    if (strncmp(argv[1], "b", 1) == 0)
    {
        part_b(argv[2]);
        return 0;
    }

    return 1;
}

void part_a(const char *filename)
{
    std::ifstream input_stream(filename);
    std::string line;
    unsigned int y = 0;
    Tracks track;
    while (std::getline(input_stream, line))
    {
        track.add_line_as_coord_y(line, y);
        ++y;
    }
    auto result = track.tick(CrashPolicy::StopTracks);
    while (!result)
    {
        result = track.tick(CrashPolicy::StopTracks);
    }
    std::cout << "Collision at " << result.value().first << " "
              << result.value().second << "\n";
}

void part_b(const char *filename)
{
    std::ifstream input_stream(filename);
    std::string line;
    unsigned int y = 0;
    Tracks track;
    while (std::getline(input_stream, line))
    {
        track.add_line_as_coord_y(line, y);
        ++y;
    }
    auto result = track.tick(CrashPolicy::RemoveCarts);
    while (track.carts_on_track() > 1)
    {
        result = track.tick(CrashPolicy::RemoveCarts);
    }
    std::cout << "Last Cart at " << track.carts.begin()->position.first << " "
              << track.carts.begin()->position.second << "\n";
}
