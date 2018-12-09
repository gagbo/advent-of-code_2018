CXX = g++
CXXFLAGS = -std=c++14
CXXWARNINGS = -Wall \
	      -Wextra \
	      -Wpedantic \
	      -Wconversion \
	      -Wsign-conversion \
	      -Wshadow

day09 : src/main.cpp src/game.cpp
	$(CXX) $(CXXFLAGS) $(CXXWARNINGS) $^ -o $@
