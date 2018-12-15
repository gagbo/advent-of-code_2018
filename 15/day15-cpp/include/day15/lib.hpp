#ifndef DAY15__LIB_HPP__
#define DAY15__LIB_HPP__

#include <string>

class Dummy {
 public:
  Dummy() = default;
  void display() const;
  inline std::string get_name() const { return name; }
 private:
  std::string name{"Dummy"};

};

#endif  // DAY15__LIB_HPP__
