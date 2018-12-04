#[macro_use]
extern crate text_io;
extern crate chrono;

use chrono::{NaiveDateTime, Timelike};
use std::collections::HashMap;
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
    let mut log_file: Log = Log::from_unsorted_log(input_lines);
    log_file.fill_guard_id();
    let guard_info = GuardInfo::from_sorted_log(&log_file);
    guard_info.strategy_1()
}

fn part_b(input_lines: io::BufReader<File>) -> usize {
    let mut log_file: Log = Log::from_unsorted_log(input_lines);
    log_file.fill_guard_id();
    let guard_info = GuardInfo::from_sorted_log(&log_file);
    guard_info.strategy_2()
}

#[derive(Debug)]
struct Log {
    entries: Vec<LogEntry>,
    is_sorted: bool,
}

#[derive(Debug, Clone)]
struct LogEntry {
    time_stamp: NaiveDateTime,
    guard_id: Option<usize>,
    event: Result<EventType, String>,
}

#[derive(Debug, Clone)]
enum EventType {
    BeginShift,
    SleepIn,
    WakeUp,
}

#[derive(Debug)]
struct Guard {
    id: usize,
    total_minutes_slept: usize,
    minutes_asleep_table: Vec<usize>,
}

#[derive(Debug)]
struct GuardInfo {
    duty_guards: HashMap<usize, Guard>,
}

impl LogEntry {
    pub fn from_str(input_line: &str) -> Self {
        let parts: Vec<&str> = input_line.split(|c| c == '[' || c == ']').collect();
        assert_eq!(parts.len(), 3);

        let time_stamp = NaiveDateTime::parse_from_str(parts[1], "%Y-%m-%d %H:%M").unwrap();
        let mut words = parts[2].split_whitespace();
        let event = match words.next() {
            Some("Guard") => Ok(EventType::BeginShift),
            Some("falls") => Ok(EventType::SleepIn),
            Some("wakes") => Ok(EventType::WakeUp),
            _ => Err(String::from("Could not parse")),
        };
        let guard_id = match event {
            Ok(EventType::BeginShift) => {
                let word = words.next().unwrap();
                let id: usize;
                scan!(word.bytes() => "#{}", id);
                Some(id)
            }
            _ => None,
        };

        LogEntry {
            time_stamp: time_stamp,
            event: event,
            guard_id: guard_id,
        }
    }
}

impl Log {
    pub fn from_unsorted_log(input_lines: io::BufReader<File>) -> Self {
        let mut entries: Vec<LogEntry> = Vec::new();
        for line in input_lines.lines() {
            let line = line.unwrap();
            let new_entry = LogEntry::from_str(&line);
            entries.push(new_entry);
        }
        entries.sort_unstable_by(|a, b| a.time_stamp.cmp(&b.time_stamp));
        Log {
            entries,
            is_sorted: true,
        }
    }

    pub fn sort(&mut self) {
        self.entries
            .sort_unstable_by(|a, b| a.time_stamp.cmp(&b.time_stamp));
        self.is_sorted = true;
    }

    pub fn fill_guard_id(&mut self) {
        if !self.is_sorted {
            self.sort();
        }

        let mut current_id = None;
        for mut entry in &mut self.entries {
            match entry.clone().event.unwrap() {
                EventType::BeginShift => {
                    current_id = entry.guard_id;
                }
                _ => {
                    entry.guard_id = current_id;
                }
            }
        }
    }
}

impl Guard {
    pub fn new(id: usize) -> Self {
        let minutes_asleep_table: Vec<usize> = vec![0; 60];
        Guard {
            id,
            total_minutes_slept: 0,
            minutes_asleep_table,
        }
    }

    pub fn add_nap(&mut self, start_minute: usize, duration: usize) {
        self.total_minutes_slept += duration;
        for i in start_minute..start_minute + duration {
            self.minutes_asleep_table[i] += 1;
        }
    }

    pub fn most_asleep_minute(&self) -> (usize, &usize) {
        self.minutes_asleep_table
            .iter()
            .enumerate()
            .max_by_key(|&(_, item)| item)
            .unwrap()
    }
}

impl GuardInfo {
    pub fn from_sorted_log(log: &Log) -> Self {
        if !log.is_sorted {
            panic!();
        }

        let mut sleep_start: Option<usize> = None;
        let mut sleep_duration: Option<usize>;
        let mut duty_guards: HashMap<usize, Guard> = HashMap::new();

        for entry in &log.entries {
            match entry.event.as_ref().unwrap() {
                EventType::SleepIn => {
                    match sleep_start {
                        Some(_) => panic!("Reached a sleep start event before a sleep end event"),
                        None => sleep_start = Some(entry.time_stamp.minute() as usize),
                    };
                }
                EventType::WakeUp => {
                    sleep_duration = Some(
                        entry.time_stamp.minute() as usize
                            - sleep_start
                                .expect("Reached a sleep end event before a sleep start event"),
                    );
                    let guard_id = entry.guard_id.unwrap();
                    let guard = duty_guards.entry(guard_id).or_insert(Guard::new(guard_id));
                    guard.add_nap(sleep_start.unwrap(), sleep_duration.unwrap());
                    sleep_start = None;
                }
                _ => {}
            }
        }
        GuardInfo { duty_guards }
    }

    pub fn strategy_1(&self) -> usize {
        let sleepiest_guard = self
            .duty_guards
            .values()
            .max_by(|g_a, g_b| g_a.total_minutes_slept.cmp(&g_b.total_minutes_slept))
            .unwrap();
        let sleepiest_minute = sleepiest_guard
            .minutes_asleep_table
            .iter()
            .enumerate()
            .max_by_key(|&(_, item)| item)
            .unwrap();
        println!(
            "Guard : {} Minute : {} Result : {}",
            sleepiest_guard.id,
            sleepiest_minute.0,
            sleepiest_guard.id * sleepiest_minute.0
        );
        sleepiest_guard.id * sleepiest_minute.0
    }

    pub fn strategy_2(&self) -> usize {
        let sleepiest_guard = self
            .duty_guards
            .values()
            .max_by(|g_a, g_b| g_a.most_asleep_minute().1.cmp(&g_b.most_asleep_minute().1))
            .unwrap();
        println!(
            "Guard : {} Minutes tuple : {:?} Result : {}",
            sleepiest_guard.id,
            sleepiest_guard.most_asleep_minute(),
            sleepiest_guard.id * sleepiest_guard.most_asleep_minute().0
        );
        sleepiest_guard.id * sleepiest_guard.most_asleep_minute().0
    }
}
