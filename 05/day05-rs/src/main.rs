use itertools::Itertools;
use std::cmp;
use std::env;
use std::fs::File;
use std::io;
use std::io::BufRead;

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
    let polymer = input_lines.lines().next().unwrap().unwrap();
    println!("Initial Polymer len : {}", polymer.len());
    let mut run_again = true;
    let mut end_polymer = polymer;

    while run_again {
        // TODO : find a way to use coalesce in a destructive way ???
        // Currently I have to replace self-destructing pairs with a non-valid char ('0') which I
        // filter out after. The problem with this approach is that I have to loop over the string
        // an undefined number of times
        let new_polymer: String = end_polymer
            .chars()
            .coalesce(|a, b| {
                if a.eq_ignore_ascii_case(&b) && a.is_ascii_uppercase() != b.is_ascii_uppercase() {
                    Ok('0')
                } else {
                    Err((a, b))
                }
            })
            .filter(|x| x != &'0')
            .join("");
        run_again = end_polymer != new_polymer;
        end_polymer = new_polymer;
    }

    // Old version which did not compile because of trying to borrow (ch_next = iter.peek) iter
    // after it is moved (for ch in iter). No idea how to make this work.
    // while run_again {
    //     let mut iter = end_polymer.bytes().peekable();
    //     let mut new_polymer = String::new();
    //     for ch in iter {
    //         let ch_next = iter.peek();
    //         match ch_next {
    //             Some(ch_next) => {
    //                 if ch.eq_ignore_ascii_case(ch_next)
    //                     && ch.is_ascii_uppercase() != ch_next.is_ascii_uppercase()
    //                 {
    //                     // skip the characters
    //                     iter.next();
    //                     iter.next();
    //                 } else {
    //                     new_polymer.push(ch as char);
    //                 }
    //             }
    //             None => continue,
    //         }
    //     }
    //     run_again = end_polymer != new_polymer;
    //     end_polymer = new_polymer;
    // }

    println!("Final Polymer len : {}", end_polymer.len());
    end_polymer.len()
}

fn part_b(input_lines: io::BufReader<File>) -> usize {
    let polymer = input_lines.lines().next().unwrap().unwrap();
    println!("Initial Polymer len : {}", polymer.len());
    let mut current_min = usize::max_value();

    for removed_char in "abcdefghijklmnopqrstuvwxyz".chars() {
        let mut run_again = true;
        let mut end_polymer = polymer
            .chars()
            .filter(|x| !x.eq_ignore_ascii_case(&removed_char))
            .join("");
        println!(
            "Initial Polymer len removing {} : {}",
            removed_char,
            end_polymer.len()
        );

        if end_polymer.len() == polymer.len() {
            println!("No occurrence of {} in the polymer, skipping...", removed_char);
            continue;
        }

        while run_again {
            let new_polymer: String = end_polymer
                .chars()
                .coalesce(|a, b| {
                    if a.eq_ignore_ascii_case(&b)
                        && a.is_ascii_uppercase() != b.is_ascii_uppercase()
                    {
                        Ok('0')
                    } else {
                        Err((a, b))
                    }
                })
                .filter(|x| x != &'0')
                .join("");
            run_again = end_polymer != new_polymer;
            end_polymer = new_polymer;
        }
        let current_len = end_polymer.len();
        println!(
            "Final Polymer len removing {} : {}",
            removed_char, current_len
        );
        current_min = cmp::min(current_len, current_min);
    }
    current_min
}
