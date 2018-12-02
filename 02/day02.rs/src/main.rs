use std::collections::HashMap;
use std::env;
use std::fs::File;
use std::io;
use std::io::BufRead;
use std::io::Write;

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
    let mut letter_twice = 0usize;
    let mut letter_thrice = 0usize;
    let mut letter_dict = HashMap::new();
    letter_dict.reserve(26);

    for line in input_lines.lines() {
        let line = line.unwrap();
        for char in line.chars() {
            let counter = letter_dict.entry(char).or_insert(0);
            *counter += 1;
        }

        let mut has_letter_twice = false;
        let mut has_letter_thrice = false;

        for (_, val) in letter_dict.iter() {
            match val {
                2 => has_letter_twice = true,
                3 => has_letter_thrice = true,
                _ => (),
            };
        }

        letter_twice = if has_letter_twice {
            letter_twice + 1
        } else {
            letter_twice
        };
        letter_thrice = if has_letter_thrice {
            letter_thrice + 1
        } else {
            letter_thrice
        };
        letter_dict.clear();
    }
    letter_thrice * letter_twice
}

fn part_b(input_lines: io::BufReader<File>) -> usize {
    let mut input_lines = input_lines
        .lines()
        .map(|line| line.unwrap())
        .collect::<Vec<String>>();

    while let Some((line_i, lines_j)) = input_lines.split_first() {
        for line_j in lines_j {
            let diff = different_char_count(line_i, line_j);
            if diff == 1 {
                println!("Found our candidates");
                let i_chars = line_i.chars();
                let mut j_chars = line_j.chars();
                for i_char in i_chars {
                    let j_char = j_chars.next().unwrap();
                    if i_char == j_char {
                        print!("{}", i_char);
                    }
                }
                print!("\n");
                io::stdout().flush().unwrap();
                return 0;
            }
        }
        input_lines = lines_j.to_vec();
    }
    1
}

/// Returns the count of different characters between the two strings
///
/// Runs a position by position difference between the chars
///
/// If the 2 strings have different lengths, the difference in lengths
/// is automatically added to the "difference count"
///
/// # Example
/// ```
/// assert_eq!(different_char_count("qqq", "qqq"), 0);
/// assert_eq!(different_char_count("qqq", "qwq"), 1);
/// assert_eq!(different_char_count("qqq", "qqqw"), 1);
/// assert_eq!(different_char_count("q1qe", "qqq"), 2);
/// ```
fn different_char_count(string_a: &str, string_b: &str) -> usize {
    let mut result = 0;
    let mut b_chars = string_b.chars();
    for c in string_a.chars() {
        let b_char = match b_chars.next() {
            Some(c_b) => c_b,
            None => '\0',
        };
        if b_char == '\0' {
            result += 1;
            continue;
        }
        if c != b_char {
            result += 1;
        }
    }

    for _ in b_chars {
        result += 1;
    }
    result
}

#[test]
fn test_different_char() {
    assert_eq!(different_char_count("qqq", "qqq"), 0);
    assert_eq!(different_char_count("qqq", "qwq"), 1);
    assert_eq!(different_char_count("qqq", "qqqw"), 1);
    assert_eq!(different_char_count("qeq", "qqqw"), 2);
    assert_eq!(different_char_count("qeq", "qqqwds"), 4);
    assert_eq!(different_char_count("qqq2", "qqq"), 1);
    assert_eq!(different_char_count("q1qe", "qqq"), 2);
    assert_eq!(different_char_count("qeqfdsa", "qqq"), 5);
    assert_eq!(different_char_count("fdasq", "qfdas"), 5);
}
