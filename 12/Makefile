CXX = g++
CXXFLAGS = -std=c++17 \
	   -O3
CXXWARNINGS = -Wall \
	      -Wextra \
	      -Wpedantic \
	      -Wconversion \
	      -Wsign-conversion \
	      -Wshadow

day12 : day12-cpp/src/main.cpp day12-cpp/src/life.cpp
	$(CXX) $(CXXFLAGS) $(CXXWARNINGS) $^ -o $@

test_a : day12
	./$< a test_input.txt

part_a : day12
	./$< a input.txt

part_b : day12
	./$< b input.txt
