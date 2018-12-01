# Advent of Code 2018
Solutions to Advent of Code 2018, in whichever languages I feel like doing (probably Rust / Modern C++ / Python3)

[Reference](https://adventofcode.com)

# Usage

```sh
./aoc_2018.sh -d DAY [args]
```

The launcher script needs one flagged argument :

- `-d DAY` is the number of the day for which we want to launch the case
- All `[args]` are forwarded to the corresponding `$day/day$day` executable.

This means all the programs should be symlinked to this location whichever
language/buildsystem is used.
