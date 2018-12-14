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

SOURCES = day14-cpp/src/main.cpp \
		  day14-cpp/src/game.cpp

HEADERS = day14-cpp/src/game.hpp

day14 : $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(CXXDEBUGFLAGS) $(CXXWARNINGS) $(SOURCES) -o $@

test_a : test_a_0 test_a_1 test_a_2 test_a_3

test_a_0: day14
	@echo "First stop at 9 recipes"
	@echo "Expecting 5158916779"
	@./$< a 9
	@echo ""

test_a_1: day14
	@echo "First stop at 5 recipes"
	@echo "Expecting 0124515891"
	@./$< a 5
	@echo ""

test_a_2: day14
	@echo "First stop at 18 recipes"
	@echo "Expecting 9251071085"
	@./$< a 18
	@echo ""

test_a_3: day14
	@echo "First stop at 2018 recipes"
	@echo "Expecting 5941429882"
	@./$< a 2018
	@echo ""


part_a : day14
	./$< a 824501

test_b : test_b_0 test_b_1 test_b_2 test_b_3

test_b_0: day14
	@echo "Looking for 51589"
	@echo "Expecting 9"
	@./$< b 51589
	@echo ""

test_b_1: day14
	@echo "Looking for 01245"
	@echo "Expecting 5"
	@./$< b 01245
	@echo ""

test_b_2: day14
	@echo "Looking for 92510"
	@echo "Expecting 18"
	@./$< b 92510
	@echo ""

test_b_3: day14
	@echo "Looking for 59414"
	@echo "Expecting 2018"
	@./$< b 59414
	@echo ""

part_b : day14
	./$< b 824501
