use std::collections::HashSet;
use std::env;
use std::fs::File;
use std::io;
use std::io::BufRead;

fn main() {
    println!("Hello, world!");
    let args = env::args().collect::<Vec<String>>();
    if args.len() < 3 {
        panic!("Not enough arguments")
    }

    let input_filename = &args[2];
    let input_lines = read_input(input_filename).unwrap();

    let result = match args[1].as_str() {
        "a" => part_a(input_lines, 0),
        "b" => part_b(input_lines, 0),
        _ => panic!("Expecting a or b as 1st argument"),
    };
    println!("{}", result);
}

fn read_input(filename: &str) -> Result<io::BufReader<File>, io::Error> {
    let file = File::open(filename)?;
    let buf_reader = io::BufReader::new(file);
    Ok(buf_reader)
}

fn part_a(input_lines: io::BufReader<File>, starting_frequence: isize) -> isize {
    let mut freq = starting_frequence;
    for line in input_lines.lines() {
        let offset: isize = line.unwrap().parse().unwrap();
        freq += offset;
    }
    freq
}

fn part_b(input_lines: io::BufReader<File>, starting_frequence: isize) -> isize {
    let mut freq = starting_frequence;
    let mut visited_frequencies = HashSet::new();
    visited_frequencies.insert(freq);
    let offset_values = input_lines
        .lines()
        .map(|line| line.unwrap().parse::<isize>().unwrap())
        .collect::<Vec<isize>>();
    loop {
        for offset in &offset_values {
            freq += offset;
            if !visited_frequencies.insert(freq) {
                println!("Duplicate found : {}", freq);
                return freq;
            }
        }
    }
}

// Kept to understand why I can't make a version which does NOT move input_lines.
// input_lines.lines() takes input_lines to transform it into an iterator. I
// would like to know if there's a way/method to be able to access input_lines
// multiple times without moving the value.

// fn part_c(input_lines: io::BufReader<File>, starting_frequence: isize) -> isize {
//     let mut freq = starting_frequence;
//     let mut visited_frequencies = HashSet::new();
//     visited_frequencies.insert(freq);
//     loop {
//         for line in input_lines.lines() {
//             let offset: isize = line.unwrap().parse().unwrap();
//             freq += offset;
//             if !visited_frequencies.insert(freq) {
//                 println!("Duplicate found : {}", freq);
//                 return freq;
//             }
//         }
//     }
// }
