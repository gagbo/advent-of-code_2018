//use itertools::Itertools;
use std::collections::HashSet;
use std::env;
use std::fs::File;
use std::io;
use std::io::BufRead;
extern crate time;
#[macro_use]
extern crate text_io;

const PART_B_CONST: usize = 10000;

fn main() {
    let args = env::args().collect::<Vec<String>>();
    if args.len() < 3 {
        panic!("Not enough arguments")
    }

    let input_filename = &args[2];
    let input_lines = read_input(input_filename).unwrap();

    let start_t = time::precise_time_s();
    let result = match args[1].as_str() {
        "a" => {
            println!("Calling Part A");
            part_a(input_lines)
        }
        "b" => {
            println!("Calling Part B");
            part_b(input_lines)
        }
        _ => panic!("Expecting a or b as 1st argument"),
    };
    let end_t = time::precise_time_s();
    println!("{} : in {} seconds", result, end_t - start_t);
}

fn read_input(filename: &str) -> Result<io::BufReader<File>, io::Error> {
    let file = File::open(filename)?;
    let buf_reader = io::BufReader::new(file);
    Ok(buf_reader)
}

fn part_a(input_lines: io::BufReader<File>) -> usize {
    let mut points: HashSet<Point> = HashSet::new();
    for (i, line) in input_lines.lines().enumerate() {
        let point = Point::from_str_with_id(i as isize, &line.unwrap());
        points.insert(point);
    }

    let max_height = points.iter().max_by(|a, b| a.y.cmp(&b.y)).unwrap().y as usize + 1;
    let max_width = points.iter().max_by(|a, b| a.x.cmp(&b.x)).unwrap().x as usize + 1;
    println!(
        "Size of the grid : {}x{} {}",
        max_width,
        max_height,
        max_height * max_width
    );

    // Naively find the closest voronoi center
    // let mut grid: HashSet<Point> = HashSet::new();
    let mut grid: Vec<Vec<isize>> = vec![vec![-1; max_height]; max_width];
    for x in 0..max_width as isize {
        for y in 0..max_height as isize {
            let mut closest_dist = max_width * max_height as usize;
            let mut point_id = -1;

            for center in &points {
                let distance = Point { x, y, id: -1 }.dist(&center);

                if distance == closest_dist {
                    point_id = -1;
                }
                if distance < closest_dist {
                    point_id = center.id;
                    closest_dist = distance;
                }
            }

            grid[x as usize][y as usize] = point_id;
        }
    }

    println!("Before filtering :");
    for y in 0..max_height {
        for x in 0..max_width {
            print!("{:02} ", grid[x][y]);
        }
        println!("");
    }
    for candidate_id in 0..points.len() {
        let cell_size = grid.iter().fold(0, |acc, grid_line| {
            acc + grid_line.iter().fold(0, |acc2, &point_id| {
                if point_id > 0 && point_id as usize == candidate_id {
                    acc2 + 1
                } else {
                    acc2
                }
            })
        });
        println!("Cell {} : {}", candidate_id, cell_size);
    }

    // Get rid of anything that touches the border
    for y in 0..max_height {
        let bad_id = grid[0][y];
        let bad_id_too = grid[max_width - 1][y];
        for point_id in &mut grid.iter_mut().flatten() {
            if *point_id == bad_id || *point_id == bad_id_too {
                *point_id = -1;
            }
        }
    }

    for x in 0..max_width {
        let bad_id = grid[x][0];
        let bad_id_too = grid[x][max_height - 1];
        for point_id in &mut grid.iter_mut().flatten() {
            if *point_id == bad_id || *point_id == bad_id_too {
                *point_id = -1;
            }
        }
    }

    // Get the biggest cell
    println!("After filtering :");
    let mut biggest_cell = 0usize;
    for candidate_id in 0..points.len() {
        let cell_size = grid.iter().fold(0, |acc, grid_line| {
            acc + grid_line.iter().fold(0, |acc2, &point_id| {
                if point_id > 0 && point_id as usize == candidate_id {
                    acc2 + 1
                } else {
                    acc2
                }
            })
        });
        println!("Cell {} : {}", candidate_id, cell_size);
        biggest_cell = std::cmp::max(biggest_cell, cell_size);
    }
    biggest_cell
}

fn part_b(input_lines: io::BufReader<File>) -> usize {
    let mut points: HashSet<Point> = HashSet::new();
    for (i, line) in input_lines.lines().enumerate() {
        let point = Point::from_str_with_id(i as isize, &line.unwrap());
        points.insert(point);
    }

    let max_height = points.iter().max_by(|a, b| a.y.cmp(&b.y)).unwrap().y as usize + 1;
    let max_width = points.iter().max_by(|a, b| a.x.cmp(&b.x)).unwrap().x as usize + 1;
    println!(
        "Size of the grid : {}x{} {}",
        max_width,
        max_height,
        max_height * max_width
    );

    // Naively find the closest voronoi center
    // let mut grid: HashSet<Point> = HashSet::new();
    let mut grid: Vec<Vec<usize>> = vec![vec![0; max_height]; max_width];
    let mut acc: usize = 0;
    for x in 0..max_width as isize {
        for y in 0..max_height as isize {
            let mut summed_dist: usize = 0;

            for center in &points {
                let distance = Point { x, y, id: -1 }.dist(&center);
                summed_dist += distance;
            }
            grid[x as usize][y as usize] = summed_dist;
            if summed_dist < PART_B_CONST {
                acc += 1;
            }
        }
    }
    acc
}

#[derive(Debug, Copy, Clone, PartialEq, Eq, Hash)]
struct Point {
    x: isize,
    y: isize,
    id: isize,
}

impl Point {
    pub fn from_str_with_id(id: isize, line: &str) -> Self {
        let x: isize;
        let y: isize;
        scan!(line.bytes() => "{}, {}", x, y);
        Point { x, y, id }
    }

    pub fn dist(&self, other: &Self) -> usize {
        ((self.x - other.x).abs() + (self.y - other.y).abs()) as usize
    }
}

#[cfg(test)]
#[test]
pub fn example_part_a_from_aoc() {
    let input_lines = read_input("../test_input.txt").unwrap();
    assert_eq!(part_a(input_lines), 17);
}

#[cfg(test)]
#[test]
pub fn result_part_a_from_aoc() {
    let input_lines = read_input("../input.txt").unwrap();
    assert_eq!(part_a(input_lines), 3989);
}

#[cfg(test)]
#[test]
pub fn result_part_b_from_aoc() {
    let input_lines = read_input("../input.txt").unwrap();
    assert_eq!(part_b(input_lines), 49715);
}
