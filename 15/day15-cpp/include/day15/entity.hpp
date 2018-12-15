#ifndef DAY15_ENTITY_HPP__
#define DAY15_ENTITY_HPP__

#include <memory>
#include <utility>

static const int STARTING_HP = 200;
static const int ATTACK_FORCE = 3;
typedef std::pair<unsigned int, unsigned int> pos_t;

/**
 * @brief Entity holds all the information about the stuff on a Board
 *
 */
class Entity {
 public:
  /**
   * @brief Enumeration of the different types on Entity on the Board
   *
   */
  enum class Type {
    ELF,
    GOBLIN,
    WALL,
    VOID,
  };

  Entity(char symbol);

  /**
   * @brief Receive a hit and die if HP <= 0
   *
   * @param hit_power the force of the received hit
   */
  void receive_hit(int hit_power);

  /**
   * @brief Get the type attribute
   *
   * @return Type type of the Entity
   */
  inline Type get_type() const { return type; }
  /**
   * @brief Get the position attribute
   *
   * @return const pos_t  position of the Entity
   */
  inline const pos_t pos() const { return position; }
  /**
   * @brief Get the target attribute
   *
   * @return std::weak_ptr<Entity> Pointer to Entity's current target
   */
  inline std::weak_ptr<Entity> get_target() const { return target; }

 protected:
  /**
   * @brief Hit points of the Entity
   *
   */
  int life_points{STARTING_HP};
  /**
   * @brief Attack force of the Entity
   *
   */
  int force{ATTACK_FORCE};
  /**
   * @brief Target of the Entity
   *
   */
  std::weak_ptr<Entity> target{};
  /**
   * @brief Position of the Entity in the Board
   *
   */
  pos_t position{0, 0};
  /**
   * @brief Type of the Entity
   *
   */
  Type type;
};

#endif  // DAY15_ENTITY_HPP__
