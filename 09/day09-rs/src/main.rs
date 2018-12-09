use std::collections::VecDeque;
use std::env;
use std::fmt;
use std::fs::File;
use std::io;
use std::io::BufRead;

#[macro_use]
extern crate text_io;

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
    let players: usize;
    let marbles: usize;
    let line = input_lines.lines().next().unwrap().unwrap();
    scan!(line.bytes() => "{} players; last marble is worth {} points", players, marbles);
    println!(
        "{} players are fighting until marble {} is placed",
        players, marbles
    );

    let mut game = Game::new(players, marbles);
    game.play();
    game.winner().1.to_string()
}

fn part_b(input_lines: io::BufReader<File>) -> String {
    let players: usize;
    let marbles: usize;
    let line = input_lines.lines().next().unwrap().unwrap();
    scan!(line.bytes() => "{} players; last marble is worth {} points", players, marbles);
    println!(
        "{} players are fighting until marble {} is placed",
        players,
        marbles * 100
    );

    let mut game = Game::new(players, marbles * 100);
    game.play();
    game.winner().1.to_string()
}

struct Game {
    scores: Vec<usize>,
    circle: VecDeque<usize>,
    circle_cursor: usize,
    current_turn: usize,
    player_cursor: usize,
    last_marble: usize,
    manual_mod_count: usize,
}

impl fmt::Debug for Game {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Game scores :\n").unwrap();
        write!(f, "{:?}\n", self.scores).unwrap();
        write!(f, "Game circle state :\n").unwrap();
        match self.circle.len() {
            1 => write!(f, "[-]: ").unwrap(),
            _ => write!(f, "[{}]: ", self.current_turn).unwrap(),
        };

        for (i, marble) in self.circle.iter().enumerate() {
            if i == self.circle_cursor {
                write!(f, "_({})_ ", marble).unwrap();
            } else {
                write!(f, "{} ", marble).unwrap();
            }
        }
        write!(f, "\n")
    }
}

impl Game {
    pub fn new(players: usize, last_marble: usize) -> Self {
        let circle_size = 1 + last_marble - (last_marble / 23);
        let mut first_circle = VecDeque::<usize>::with_capacity(circle_size);
        first_circle.insert(0, 0);
        println!("Capacity reserved : {}", first_circle.capacity());
        Game {
            scores: vec![0; players],
            circle: first_circle,
            circle_cursor: 0,
            current_turn: 0,
            player_cursor: 0,
            last_marble,
            manual_mod_count: 0,
        }
    }

    fn do_one_turn(&mut self) {
        self.current_turn += 1;
        self.manual_mod_count += 1;
        // Do not advance player_cursor on first turn
        if self.circle.len() > 1 {
            if self.player_cursor == self.scores.len() - 1 {
                self.player_cursor = 0;
            } else {
                self.player_cursor += 1;
            }
        }

        if self.manual_mod_count == 23 {
            // Add points
            self.scores[self.player_cursor] += self.current_turn;
            let remove_index = if self.circle_cursor < 7 {
                self.circle.len() + self.circle_cursor - 7
            } else {
                self.circle_cursor - 7
            };
            self.scores[self.player_cursor] += self.circle.remove(remove_index).unwrap();
            self.circle_cursor = remove_index;
            self.manual_mod_count = 0;
        } else {
            // Insert marble
            let mut insert_index = self.circle_cursor + 2;
            if insert_index >= self.circle.len() {
                insert_index -= self.circle.len();
            }
            self.circle.insert(insert_index, self.current_turn);
            self.circle_cursor = insert_index;
        }

        if self.current_turn % 25000 == 0 {
            println!("Turn {} done", self.current_turn);
        }
    }

    pub fn play(&mut self) {
        loop {
            if self.current_turn > self.last_marble {
                break;
            }
            self.do_one_turn();
        }
    }

    pub fn winner(&self) -> (usize, &usize) {
        self.scores
            .iter()
            .enumerate()
            .max_by(|(_, score_a), (_, score_b)| score_a.cmp(&score_b))
            .unwrap()
    }
}

#[cfg(test)]
#[test]
pub fn example_part_a_from_aoc() {
    let input_lines = read_input("../test_input_easy.txt").unwrap();
    assert_eq!(part_a(input_lines), "32");

    let input_lines = read_input("../test_input_0.txt").unwrap();
    assert_eq!(part_a(input_lines), "8317");

    let input_lines = read_input("../test_input_1.txt").unwrap();
    assert_eq!(part_a(input_lines), "146373");

    let input_lines = read_input("../test_input_2.txt").unwrap();
    assert_eq!(part_a(input_lines), "2764");

    let input_lines = read_input("../test_input_3.txt").unwrap();
    assert_eq!(part_a(input_lines), "54718");

    let input_lines = read_input("../test_input_4.txt").unwrap();
    assert_eq!(part_a(input_lines), "37305");
}

#[cfg(test)]
#[test]
pub fn answer_part_a_from_aoc() {
    let input_lines = read_input("../input.txt").unwrap();
    assert_eq!(part_a(input_lines), "390093");
}

// #[cfg(test)]
// #[test]
// pub fn answer_part_b_from_aoc() {
//     let input_lines = read_input("../input.txt").unwrap();
//     assert_eq!(part_b(input_lines), "33891");
// }
