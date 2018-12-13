#include "tracks.hpp"
#include <iostream>

void Cart::advance() {
  switch (direction) {
  case CartDir::N:
    position.second -= 1;
    break;
  case CartDir::E:
    position.first += 1;
    break;
  case CartDir::S:
    position.second += 1;
    break;
  case CartDir::W:
    position.first -= 1;
    break;
  default:
    std::cerr << "Unknown direction, not moving Cart \n";
    break;
  }
}

void Cart::take_intersection_action() {
  switch (nextAction) {
  case IntersectionAction::GoLeft:
    turn_left();
    nextAction = IntersectionAction::GoStraight;
    break;
  case IntersectionAction::GoRight:
    turn_right();
    nextAction = IntersectionAction::GoLeft;
    break;
  case IntersectionAction::GoStraight:
    nextAction = IntersectionAction::GoRight;
    break;
  default:
    std::cerr << "Unknown action !\n";
    break;
  }
}

void Cart::turn_left() {
  switch (direction) {
  case CartDir::N:
    direction = CartDir::W;
    break;
  case CartDir::E:
    direction = CartDir::N;
    break;
  case CartDir::S:
    direction = CartDir::E;
    break;
  case CartDir::W:
    direction = CartDir::S;
    break;
  default:
    std::cerr << "Unknown direction !\n";
    break;
  }
}

void Cart::turn_right() {
  switch (direction) {
  case CartDir::N:
    direction = CartDir::E;
    break;
  case CartDir::E:
    direction = CartDir::S;
    break;
  case CartDir::S:
    direction = CartDir::W;
    break;
  case CartDir::W:
    direction = CartDir::N;
    break;
  default:
    std::cerr << "Unknowm direction !\n";
    break;
  }
}

std::optional<pos_t> Tracks::tick(CrashPolicy policy) {
  auto result = tick_start(policy);
  if (result)
    return result;
  tick_end();
  ++ticks;
  return std::nullopt;
}

std::optional<pos_t> Tracks::tick_start(CrashPolicy policy) {
  next_carts.clear();
  while (carts.size()) {
    auto cart = carts.extract(carts.begin()).value();
    cart.advance();
    if (is_intersection(cart.position))
      cart.take_intersection_action();
    auto turn = is_turn(cart.position);
    if (turn)
      cart.direction = turn->next_dir(cart.direction);
    auto result = next_carts.insert(cart);
    bool collision;

    switch (policy) {
    case CrashPolicy::StopTracks:
      collision = carts.find(cart) != carts.end() || !result.second;
      if (collision)
        return cart.position;
      break;
    case CrashPolicy::RemoveCarts:
      auto old_cart = carts.find(cart);
      collision = old_cart != carts.end();
      if (collision) {
        carts.erase(old_cart);
        next_carts.erase(result.first);
      } else if (!result.second) {
        next_carts.erase(result.first);
      }
    }
  }
  return std::nullopt;
}

void Tracks::tick_end() { carts = next_carts; }

bool Tracks::is_intersection(const pos_t &position) const {
  return intersections.find(position) != intersections.end();
}

std::optional<Turn> Tracks::is_turn(const pos_t &position) const {
  auto it_turn = turns.find(position);
  if (it_turn != turns.end()) {
    return it_turn->second;
  }
  return std::nullopt;
}

CartDir Turn::next_dir(CartDir incoming_direction) const {
  switch (type) {
  case TurnType::YisX:
    switch (incoming_direction) {
    case CartDir::N:
      return CartDir::E;
    case CartDir::W:
      return CartDir::S;
    case CartDir::S:
      return CartDir::W;
    case CartDir::E:
      return CartDir::N;
    default:
      std::cerr << "Unknown direction, not changing\n";
      return incoming_direction;
    }
  case TurnType::YisMinusX:
    switch (incoming_direction) {
    case CartDir::N:
      return CartDir::W;
    case CartDir::W:
      return CartDir::N;
    case CartDir::S:
      return CartDir::E;
    case CartDir::E:
      return CartDir::S;
    default:
      std::cerr << "Unknown direction, not changing\n";
      return incoming_direction;
    }
  default:
    std::cerr << "Unknown turn type, not changing\n";
    return incoming_direction;
  }
}

void Tracks::add_line_as_coord_y(const std::string &line,
                                 const unsigned int &fixed_y) {
  unsigned int x = 0;
  for (auto c : line) {
    pos_t current_pos = std::make_pair(x, fixed_y);
    switch (c) {
    case '/':
      turns[current_pos] = Turn(TurnType::YisX);
      break;
    case '\\':
      turns[current_pos] = Turn(TurnType::YisMinusX);
      break;
    case '+':
      intersections.insert(current_pos);
      break;
    case '>':
      carts.insert(Cart(current_pos, CartDir::E));
      break;
    case '<':
      carts.insert(Cart(current_pos, CartDir::W));
      break;
    case '^':
      carts.insert(Cart(current_pos, CartDir::N));
      break;
    case 'v':
      carts.insert(Cart(current_pos, CartDir::S));
      break;
    default:
      break;
    }
    ++x;
  }
}