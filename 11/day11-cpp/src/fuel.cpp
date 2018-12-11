#include "fuel.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

const size_t FuelGrid::GRID_SIZE = 300;

FuelGrid::FuelGrid(int grid_serial) : serial(grid_serial) {
    for (size_t x = 0; x < GRID_SIZE; ++x) {
        std::vector<std::unique_ptr<FuelCell>> line_x;
        for (size_t y = 0; y < GRID_SIZE; ++y) {
            line_x.push_back(std::make_unique<FuelCell>(x + 1, y + 1));
        }
        _grid.push_back(std::move(line_x));
    }
}

void FuelCell::compute_value(int serial) {
    int rack_id = _x + 10;
    int power_level = rack_id * _y;
    power_level += serial;
    power_level *= rack_id;

    for (int i = 0; i < 2; i++) {
        power_level -= power_level % 10;
        power_level /= 10;
    }
    power_level = power_level % 10;
    _value = power_level - 5;
}

void FuelGrid::convert_all_cells() {
    for (auto& line : _grid) {
        std::for_each(line.begin(), line.end(),
                      [&](auto& cell) { cell->compute_value(serial); });
    }
}

void FuelGrid::best_square() {
    convert_all_cells();
    size_t x_max, y_max, sq_size_max;
    int val_max = std::numeric_limits<int>::min();
    for (size_t sq_size = 1; sq_size <= 300; ++sq_size) {
        std::cout << "Testing " << sq_size << "x" << sq_size << " squares...\n";
        for (size_t x = 1; x <= GRID_SIZE; ++x) {
            for (size_t y = 1; y <= GRID_SIZE; ++y) {
                int current_val = square_value(x, y, sq_size);
                if (current_val > val_max) {
                    x_max = x;
                    y_max = y;
                    sq_size_max = sq_size;
                    val_max = current_val;
                }
            }
        }
    }

    std::cout << "Best square is (" << x_max << ", " << y_max << ", "
              << sq_size_max << ") with a value of " << val_max << "\n";
}

void FuelGrid::best_square_one_size(size_t sq_size) {
    convert_all_cells();
    size_t x_max, y_max;
    int val_max = std::numeric_limits<int>::min();
    for (size_t x = 1; x <= GRID_SIZE; ++x) {
        for (size_t y = 1; y <= GRID_SIZE; ++y) {
            int current_val = square_value(x, y, sq_size);
            if (current_val > val_max) {
                x_max = x;
                y_max = y;
                val_max = current_val;
            }
        }
    }

    std::cout << "Best square is (" << x_max << ", " << y_max
              << ") with a value of " << val_max << "\n";
}

int FuelGrid::square_value(size_t x, size_t y, size_t sq_size) const {
    if (x < 1 || y < 1 || x + sq_size - 1 > GRID_SIZE ||
        y + sq_size - 1 > GRID_SIZE) {
        return std::numeric_limits<int>::min();
    }

    auto find_in_cache = square_cache.find(std::make_tuple(x, y, sq_size));
    if (find_in_cache != square_cache.end()) {
        return find_in_cache->second;
    }

    if (sq_size == 1) {
      int total_val = _grid[x-1][y-1]->value();
      square_cache.insert(std::make_pair(std::make_tuple(x, y, sq_size), total_val));
      return total_val;
    }

    int total_val = 0;
    size_t left_size = static_cast<size_t>(std::floor(sq_size / 2));
    // NW square : Always a square
    total_val += square_value(x, y, left_size);
    // SE square : Always a square
    total_val +=
        square_value(x + left_size, y + left_size, sq_size - left_size);
    // SW square : this leaves the bottom row is sq_size is odd
    total_val += square_value(x, y + left_size, left_size);
    // NE square : this leaves the right col is sq_size is odd
    total_val += square_value(x + left_size, y, left_size);

    if (sq_size - left_size != left_size) {
        size_t bottom_y = y + sq_size - 1;
        for (size_t bottom_x = x; bottom_x < x + left_size; bottom_x++) {
          // Which line is better ?
          // I think that the _grid direct access line grants quicker access
          // instruction-count wise. Also, calling it for every 1x1 requests
          // means that _grid should stay in a close enough cache, and the
          // contiguous guarantee of a std::vector<> helps a lot.
          // But maybe actually having this line
          // forces _grid to be called back in a closer cache, where my
          // square_cache is pulled to test for find() and insert()
          //
          // The square_value() is a 'slower' operation instruction-count wise I
          // think. But the mean time per instruction should be better since
          // once the cache is loaded with the 1x1 values, _grid is never called
          // anymore and everything should happen in sq_cache.
          //
          // Benchmark has spoken, memory locality of _grid wins, and HANDILY.
          // See README.md for results

            total_val += _grid[bottom_x - 1][bottom_y - 1]->value();
            /* total_val += square_value(bottom_x, bottom_y, 1); */
        }

        size_t right_x = x + sq_size - 1;
        for (size_t right_y = y; right_y < y + left_size; right_y++) {
            total_val += _grid[right_x - 1][right_y - 1]->value();
            /* total_val += square_value(right_x, right_y, 1); */
        }
    }

    square_cache.insert(std::make_pair(std::make_tuple(x, y, sq_size), total_val));
    return total_val;
}
