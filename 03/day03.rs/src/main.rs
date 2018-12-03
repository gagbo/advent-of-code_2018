use std::collections::HashSet;
use std::env;
use std::fs::File;
use std::io;
use std::io::BufRead;
#[macro_use]
extern crate text_io;

const FABRIC_SIZE: usize = 1000;

fn main() {
    let args = env::args().collect::<Vec<String>>();
    if args.len() < 3 {
        panic!("Not enough arguments")
    }

    let input_filename = &args[2];
    let input_lines = read_input(input_filename).unwrap();

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
    println!("{}", result);
}

fn read_input(filename: &str) -> Result<io::BufReader<File>, io::Error> {
    let file = File::open(filename)?;
    let buf_reader = io::BufReader::new(file);
    Ok(buf_reader)
}

fn part_a(input_lines: io::BufReader<File>) -> usize {
    let mut fabric_map: [[usize; FABRIC_SIZE]; FABRIC_SIZE] = [[0; FABRIC_SIZE]; FABRIC_SIZE];
    let mut claims = Vec::new();
    for line in input_lines.lines() {
        let line = line.unwrap();
        let new_claim = Claim::from_str(&line);
        new_claim.mark_map(&mut fabric_map);
        claims.push(new_claim);
    }
    fabric_map
        .iter()
        .map(|arr| arr.iter().filter(|&&x| x > 1).count())
        .fold(0, |acc, x| acc + x)
}

fn part_b(input_lines: io::BufReader<File>) -> usize {
    let mut fabric_map: [[usize; FABRIC_SIZE]; FABRIC_SIZE] = [[0; FABRIC_SIZE]; FABRIC_SIZE];
    let mut claims = HashSet::new();
    for line in input_lines.lines() {
        let line = line.unwrap();
        let new_claim = Claim::from_str(&line);
        new_claim.mark_map(&mut fabric_map);
        claims.insert(new_claim);
    }

    'main_outer: for claim in claims {
        for i in claim.left..claim.left + claim.width {
            for j in claim.top..claim.top + claim.height {
                if fabric_map[i][j] > 1 {
                    continue 'main_outer;
                }
            }
        }
        return claim.id;
    }
    1
}

#[derive(Debug, PartialEq, Eq, Hash)]
struct Claim {
    id: usize,
    left: usize,
    top: usize,
    width: usize,
    height: usize,
}

impl Claim {
    pub fn from_str(input_line: &str) -> Self {
        let id: usize;
        let left: usize;
        let top: usize;
        let width: usize;
        let height: usize;

        scan!(input_line.bytes() => "#{} @ {},{}: {}x{}", id, left, top, width, height);
        Claim {
            id,
            left,
            top,
            width,
            height,
        }
    }

    pub fn mark_map(&self, fabric_map: &mut [[usize; FABRIC_SIZE]; FABRIC_SIZE]) {
        for i in self.left..self.left + self.width {
            for j in self.top..self.top + self.height {
                fabric_map[i][j] += 1;
            }
        }
    }
}
