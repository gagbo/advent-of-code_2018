CXX = g++
CXXFLAGS = -std=c++17 \
	   -O3
CXXWARNINGS = -Wall \
	      -Wextra \
	      -Wpedantic \
	      -Wconversion \
	      -Wsign-conversion \
	      -Wshadow

day11 : day11-cpp/src/main.cpp day11-cpp/src/fuel.cpp
	$(CXX) $(CXXFLAGS) $(CXXWARNINGS) $^ -o $@

test_a_1 : day11
	@echo "18 serial test case : 33,45 with total power of 29"
	./$< a 18

test_a_2 : day11
	@echo "42 serial test case : 21,61 with total power of 30"
	./$< a 42

part_a : day11
	./$< a 5153

test_b_1 : day11
	@echo "18 serial test case : 90,269,16 with total power of 113"
	./$< b 18

test_b_2 : day11
	@echo "42 serial test case : 232,251,12 with total power of 119"
	./$< b 42

part_b : day11
	./$< b 5153
