CXX = g++
CXXFLAGS = -std=c++17 \
	   -O3
CXXDEBUGFLAGS = -Og \
		-g
CXXWARNINGS = -Wall \
	      -Wextra \
	      -Wpedantic \
	      -Wconversion \
	      -Wsign-conversion \
	      -Wshadow

SOURCES = day13-cpp/src/main.cpp \
		  day13-cpp/src/tracks.cpp

HEADERS = day13-cpp/src/tracks.hpp

day13 : $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(CXXDEBUGFLAGS) $(CXXWARNINGS) $(SOURCES) -o $@

test_a : test_a_0 test_a_1 test_a_2

test_a_0: day13
	@echo "Basic vertical collision"
	@echo "Expecting collision in (0, 3)"
	@./$< a test_input_0.txt
	@echo ""

test_a_1: day13
	@echo "Basic horizontal collision"
	@echo "Expecting collision in (10, 0)"
	@./$< a test_input_1.txt
	@echo ""

test_a_2: day13
	@echo "AdventofCode test collision"
	@echo "Expecting collision in (7, 3)"
	@./$< a test_input_2.txt
	@echo ""

part_a : day13
	./$< a input.txt

test_b : day13
	@echo "AdventofCode test removal_on_collision"
	@echo "Expecting last cart to be (6, 4)"
	./$< b test_input_3.txt
	@echo ""

part_b : day13
	./$< b input.txt