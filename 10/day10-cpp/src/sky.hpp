#ifndef SKY_HPP__
#define SKY_HPP__
#include <vector>
#include <string>

class Star {
 public:
  Star(std::string line);
  void print() const;
  void advance();
  inline int x() const { return pos[0]; }
  inline int y() const { return pos[1]; }
  inline int fut_x() const { return pos[0] + vel[0]; }
  inline int fut_y() const { return pos[1] + vel[1]; }
 private:
  int pos[2];
  int vel[2];
};

class Sky {
 public:
  Sky(std::vector<Star>&& star_list) : stars(star_list){}
  long unsigned int get_width() const;
  long unsigned int get_height() const;
  long unsigned int get_fut_width() const;
  long unsigned int get_fut_height() const;
  long unsigned int fut_area() const { return get_fut_width() * get_fut_height(); }
  long unsigned int area() const { return get_width() * get_height(); }
  void advance();

  void print() const;

 private:
  std::vector<Star> stars;
};
#endif  //SKY_HPP__
