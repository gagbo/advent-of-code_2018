CXX = g++
CXX_FLAGS = -Wall \
	    -Wpedantic \
	    -Wshadow \
	    -Wconversion \
	    -Wsign-conversion \
	    -Wextra \
	    -fexceptions \
	    -std=c++14

day01: day01.cpp
	$(CXX) $(CXX_FLAGS) $^ -o $@

part_a: day01
	./$< a <input

part_b: day01
	./$< b <input
