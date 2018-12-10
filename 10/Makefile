CXX = g++
CXXFLAGS = -std=c++17
CXXWARNINGS = -Wall \
	      -Wextra \
	      -Wpedantic \
	      -Wconversion \
	      -Wsign-conversion \
	      -Wshadow

day10 : day10-cpp/src/main.cpp day10-cpp/src/sky.cpp
	$(CXX) $(CXXFLAGS) $(CXXWARNINGS) $^ -o $@

test : day10
	./$< a ./test_input.txt

part_a : day10
	./$< a ./input.txt
