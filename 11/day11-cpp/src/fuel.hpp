#ifndef FUEL_HPP__
#define FUEL_HPP__

#include <map>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

typedef std::tuple<size_t, size_t, size_t> sq_key_t;
typedef std::map<const sq_key_t,int> sq_cache_t;

class FuelCell {
 public:
    FuelCell();
    FuelCell(int x, int y) : _x(x), _y(y) {}
    void compute_value(int serial);
    inline int value() const { return _value; }

 private:
    int _x{0};
    int _y{0};
    int _value{0};
};

class FuelGrid {
 public:
    FuelGrid(int grid_serial);
    void best_square();
    void best_square_one_size(size_t sq_size);
    void convert_all_cells();
    // square_value assumes all cells have a correct value;
    int square_value(size_t x, size_t y, size_t sq_size) const;

    static const size_t GRID_SIZE;

 private:
    std::vector<std::vector<std::unique_ptr<FuelCell>>> _grid;
    int serial{0};
    mutable sq_cache_t square_cache{};
};

#endif  // FUEL_HPP__
