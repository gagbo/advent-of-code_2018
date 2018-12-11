Sample size is 1, and I do other things on my laptop

# Issue

Should I call my cache for every square size even 1, or should I just query my
vector containing each cell for the case where square size is 1.


See the code in day11-cpp/src/fuel.cpp : FuelGrid::square_value() for the
concerned lines.

# Conclusion
In this case, the memory locality from calling directly the underlying
`vector` in the FuelGrid structure is much, much better than "over" using the
square values cache even for 1x1 squares.


Note that this conclusion is different in the "no optimization case", so
benchmarking should always be done in Release configurations.

# No optimization
```make
CXX = g++
CXXFLAGS = -std=c++17
CXXWARNINGS = -Wall \
	      -Wextra \
	      -Wpedantic \
	      -Wconversion \
	      -Wsign-conversion \
	      -Wshadow
```

## Cache calls (map of values)
`./day11 b 5153  1124,43s user 0,80s system 99% cpu 18:52,06 total`

## Grid calls (vector of pointers)
`./day11 b 5153  1203,73s user 1,37s system 99% cpu 20:14,97 total`

# Best condititions
```make
CXX = g++
CXXFLAGS = -std=c++17 \
	   -O3
CXXWARNINGS = -Wall \
	      -Wextra \
	      -Wpedantic \
	      -Wconversion \
	      -Wsign-conversion \
	      -Wshadow
```
Also, niceness is put to -1 with `sudo time nice -n -1 ./day11 b 5153` ; 5153 is my input

Running with `sudo` also gives the count of pagefaults, and we can see the
numbers are quite similar there, so I can only assume that the
`mean time_per_instruction * instruction_count` product is much better in the
`vector` case.

## Cache calls (map of values)
```
106.17user 0.55system 1:47.57elapsed 99%CPU (0avgtext+0avgdata 713924maxresident)k
0inputs+0outputs (0major+177867minor)pagefaults 0swaps
```

## Grid calls (vector of pointers)
```
11.09user 0.33system 0:11.54elapsed 98%CPU (0avgtext+0avgdata 713924maxresident)k
0inputs+0outputs (0major+177863minor)pagefaults 0swaps
```

