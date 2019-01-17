CXX = g++
CXXFLAGS = -std=c++17 \
	   -O3
CXXNODEBUGFLAGS = -DNDEBUG
CXXDEBUGFLAGS = -Og \
		-g
CXXWARNINGS = -Wall \
	      -Wextra \
	      -Wpedantic \
	      -Wconversion \
	      -Wsign-conversion \
	      -Wshadow

SOURCES = day16-cpp/src/main.cpp \
		  day16-cpp/src/reg_machine.cpp

HEADERS = day16-cpp/src/reg_machine.hpp

day16 : $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(CXXNODEBUGFLAGS) $(CXXWARNINGS) $(SOURCES) -o $@

part_a : day16
	./$< a input.txt

part_b : day16
	./$< b input.txt
