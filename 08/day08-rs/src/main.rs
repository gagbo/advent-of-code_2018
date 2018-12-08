use std::cell::RefCell;
use std::env;
use std::fmt;
use std::fs::File;
use std::io;
use std::io::BufRead;
use std::rc::Rc;

extern crate time;

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

fn part_a(input_lines: io::BufReader<File>) -> String {
    let line = input_lines.lines().next().unwrap().unwrap();
    let tree: Tree = Tree::from_str(&line, 0);
    tree.sum_metadata().to_string()
}

fn part_b(input_lines: io::BufReader<File>) -> String {
    let line = input_lines.lines().next().unwrap().unwrap();
    let tree: Tree = Tree::from_str(&line, 0);
    tree.value().to_string()
}

struct Tree {
    id: usize,
    data: Vec<usize>,
    children: Vec<Rc<RefCell<Tree>>>,
}

impl fmt::Debug for Tree {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(
            f,
            "Node {} with {} children\n",
            self.id,
            self.children.len()
        )
        .unwrap();
        write!(f, "  Data : {} values\n", self.data.len()).unwrap();
        write!(f, "         {:?}\n", self.data).unwrap();
        write!(f, "  Children :\n").unwrap();
        for node in &self.children {
            node.borrow().fmt(f).unwrap();
        }
        write!(f, "------------------ {}\n", self.id)
    }
}

impl Tree {
    fn new(id: usize) -> Self {
        Tree {
            id,
            data: Vec::new(),
            children: Vec::new(),
        }
    }

    pub fn add_child(&mut self, child: Self) {
        self.children.push(Rc::new(RefCell::new(child)));
    }

    pub fn from_str(input: &str, root_id: usize) -> Self {
        Self::from_words(&mut input.split_whitespace(), root_id)
    }

    fn from_words(words: &mut std::str::SplitWhitespace, root_id: usize) -> Self {
        let mut tree = Self::new(root_id);
        let children_count = words
            .next()
            .unwrap()
            .parse::<usize>()
            .expect("Could not parse the number of children");
        let metadata_count = words
            .next()
            .unwrap()
            .parse::<usize>()
            .expect("Could not parse the number of metadata");

        for i in 1..children_count + 1 {
            tree.add_child(Self::from_words(words, (root_id + i) * 100));
        }

        for _ in 0..metadata_count {
            tree.data.push(
                words
                    .next()
                    .expect("Early end of file")
                    .parse::<usize>()
                    .expect("Could not parse metadata"),
            );
        }

        tree
    }

    pub fn sum_metadata(&self) -> usize {
        let mut result: usize = self.data.iter().sum();

        for child in self.children.iter() {
            result += child.borrow().sum_metadata();
        }

        result
    }

    pub fn value(&self) -> usize {
        match self.children.len() {
            0 => self.data.iter().sum(),
            _ => {
                let mut result: usize = 0;
                for data_child in self.data.iter().cloned() {
                    if data_child != 0 && data_child <= self.children.len() {
                        result += self.children[data_child - 1].borrow().value();
                    }
                }
                result
            }
        }
    }
}

#[cfg(test)]
#[test]
pub fn example_part_a_from_aoc() {
    let input_lines = read_input("../test_input.txt").unwrap();
    assert_eq!(part_a(input_lines), "138");
}

#[cfg(test)]
#[test]
pub fn answer_part_a_from_aoc() {
    let input_lines = read_input("../input.txt").unwrap();
    assert_eq!(part_a(input_lines), "37905");
}

#[cfg(test)]
#[test]
pub fn example_part_b_from_aoc() {
    let input_lines = read_input("../test_input.txt").unwrap();
    assert_eq!(part_b(input_lines), "66");
}

#[cfg(test)]
#[test]
pub fn answer_part_b_from_aoc() {
    let input_lines = read_input("../input.txt").unwrap();
    assert_eq!(part_b(input_lines), "33891");
}
