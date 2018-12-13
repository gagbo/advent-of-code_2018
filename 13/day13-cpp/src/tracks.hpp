#ifndef TRACKS_HPP__
#define TRACKS_HPP__

#include <map>
#include <optional>
#include <set>
#include <utility>

typedef std::pair<unsigned int, unsigned int> pos_t;

// To refactor inside Cart ?
// turning right is CartDir++, turning left is CartDir--
enum class CartDir
{
    N = 0,
    E,
    S,
    W,
};

class Cart
{
  public:
    enum class IntersectionAction
    {
        GoLeft,
        GoRight,
        GoStraight,
    };

    Cart(pos_t pos, CartDir dir) : position(pos), direction(dir) {}
    pos_t position{0, 0};
    CartDir direction{CartDir::N};
    IntersectionAction nextAction{IntersectionAction::GoLeft};
    void advance();
    void take_intersection_action();

    // Returns true if this' next state should be evaluated before rhs'
    inline bool evaluated_before(const Cart &rhs) const
    {
        if (position.second != rhs.position.second)
        {
            return position.second < rhs.position.second;
        }
        else
        {
            return position.first < rhs.position.first;
        }
    }

  private:
    void turn_left();
    void turn_right();
};

struct CartEvalCmp
{
    bool operator()(const Cart &lhs, const Cart &rhs) const
    {
        return lhs.evaluated_before(rhs);
    }
};

enum class TurnType
{
    YisX,
    YisMinusX,
    NotValid,
};

class Turn
{
  private:
    TurnType type{TurnType::NotValid};

  public:
    Turn() = default;
    Turn(TurnType _type) : type(_type) {}
    CartDir next_dir(CartDir incoming_direction) const;
};

enum class CrashPolicy {
    StopTracks,
    RemoveCarts,
};

class Tracks
{
  public:
    std::set<Cart, CartEvalCmp> carts{};
    unsigned int ticks{0};

    // Tick the carts on the tracks.
    std::optional<pos_t> tick(CrashPolicy policy = CrashPolicy::StopTracks);

    bool is_intersection(const pos_t &position) const;
    std::optional<Turn> is_turn(const pos_t &position) const;
    void add_line_as_coord_y(const std::string &line, const unsigned int &fixed_y);
    inline size_t carts_on_track() const { return carts.size(); }

  private:
    std::set<Cart, CartEvalCmp> next_carts{};
    std::set<pos_t> intersections{};
    std::map<pos_t, Turn> turns{};
    // Update next_carts. Return collision position or noting
    std::optional<pos_t> tick_start(CrashPolicy policy);
    // Update carts
    void tick_end();
};

#endif // TRACKS_HPP__
